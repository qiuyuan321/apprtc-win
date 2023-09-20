#include "CallClient.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QtCore/qeventloop.h>
#include "JsonUtils.h"
#include "webrtcApi.h"

//http://8.142.13.126:8090/join/421751279
//http://8.142.13.126:8090/leave/421751279/49486186
//http://8.142.13.126:8090/message/421751279/83040705

void onLocalVideoFrame(void* arg, const char* buffer, int format, int width, int height)
{
    //Conductor* conductor = (Conductor*)arg;
   //qDebug() << "onLocalVideoFrame: " << width << "x" << height;

    CallClient* client = (CallClient*)arg;
    client->onLocalVideoFrame1(buffer, format, width, height);
}

void onRemoteVideoFrame(void* arg, const char* buffer, int format, int width, int height)
{
    //Conductor* conductor = (Conductor*)arg;
    //qDebug() << "onRemoteVideoFrame: " << width << "x" << height;

    CallClient* client = (CallClient*)arg;
    client->onRemoteVideoFrame1(buffer, format, width, height);
}

void onSignalMessage(void* arg, const char* msg)
{
    CallClient* client = (CallClient*)arg;
    client->sendMessage(msg);
    qDebug() << msg;
}

void CallClient::onLocalVideoFrame1(const char* buffer, int format, int width, int height)
{
    emit localVideoFrame((void*)buffer, width, height, format);
    //qDebug() << "local: " << width << "x" << height;
}

void CallClient::onRemoteVideoFrame1(const char* buffer, int format, int width, int height)
{
    emit remoteVideoFrame((void*)buffer, width, height, format);
    //qDebug() << "remote: " << width << "x" << height;
}

CallClient::CallClient():
	_roomId(""), _clientId(""), _signaling(NULL)
{
    _roomServer = "http://8.142.13.126:8090";
    _initiator = false;
    _showFlag = false;

    //PeerConnectionApiOptions opts;
    //opts.callback.onLocalVideoFrame = onLocalVideoFrame;
    //opts.callback.onRemoteVideoFrame = onRemoteVideoFrame;
    //opts.callback.onSignalMessage = onSignalMessage;
    //opts.callbackArg = this;
    //_pc = createPeerConnection(&opts);

    //if (getPeerConnectionStatus(_pc) != 0)
    //{
    //    qDebug() << "Error", "Failed to initialize PeerConnectionFactory";
    //    //DeletePeerConnection();
    //    //return false;
    //    return;
    //}
    qDebug() << "ok";
}

CallClient::~CallClient()
{
    if (inRoom())
    {
        leaveRoom();
    }
    //destoryPeerConnection(_pc);
    //_pc = NULL;
}

void CallClient::createPeerConnection1()
{
    PeerConnectionApiOptions opts;
    opts.callback.onLocalVideoFrame = onLocalVideoFrame;
    opts.callback.onRemoteVideoFrame = onRemoteVideoFrame;
    opts.callback.onSignalMessage = onSignalMessage;
    opts.callbackArg = this;
    _pc = createPeerConnection(&opts);
}

void CallClient::destoryPeerConnection1()
{
    //stopPeerConnection(_pc);
    destoryPeerConnection(_pc);
    _pc = NULL;
}

bool CallClient::inRoom()
{
    return !_roomId.isEmpty();
}

void CallClient::joinRoom(QString roomId)
{
    if (inRoom())
    {
        leaveRoom();
        return;
    }

    QString url = _roomServer + "/join/" + roomId;
    QString reply = httpPost(url, "");
    QJsonObject jobj2 = getJsonObjectFromString(reply);
    if (jobj2.isEmpty() || jobj2.value("result") != "SUCCESS" || !jobj2["params"].isObject())
    {
        qDebug() << "joinRoom error: " << reply;
        return;
    }

    QJsonObject jobj = jobj2["params"].toObject();
    //qDebug() << "is_initiator:" << jobj["is_initiator"].toString();
    //qDebug() << "room_link:" << jobj["room_link"].toString();
    //qDebug() << "room_id:" << jobj["room_id"].toString();
    //qDebug() << "client_id:" << jobj["client_id"].toString();
    //qDebug() << "wss_url:" << jobj["wss_url"].toString();
    //qDebug() << "wss_post_url:" << jobj["wss_post_url"].toString();
    //qDebug() << "messages:" << jobj["messages"].toString();

    QString wssUrl = jobj["wss_url"].toString();
    QString wssPostUrl = jobj["wss_post_url"].toString();
    _roomId = jobj["room_id"].toString();
    _clientId = jobj["client_id"].toString();
    _signaling = new WssSignalingChannel(wssUrl, wssPostUrl, _roomId, _clientId);
    connect(_signaling, &SignalingChannel::textMessageReceived, this, &CallClient::onReceiveSignalingMessage);
    _signaling->open();

    _initiator = "true" == jobj["is_initiator"].toString().toLower() ? true : false;
    _connected = _initiator ? false : true;

    createPeerConnection1();

    if (!_initiator)
    {
        QJsonArray jsdps = jobj["messages"].toArray();
        for (int i = 0; i < jsdps.count(); i++)
        {
            qDebug() << jsdps[i].toString();
            handleSignalMessage(_pc, jsdps[i].toString().toStdString().c_str());
        }
    }
    else
    {
        startPeerConnection(_pc);
    }
    
}

void CallClient::leaveRoom()
{
    if (!inRoom()) return;

    QJsonObject jobj;
    jobj["type"] = "bye";
    if(_connected) _signaling->sendMessage(getStringFromJsonObject(jobj));
    _signaling->close();
    delete _signaling;
    _signaling = NULL;

    QString url = _roomServer + "/leave/" + _roomId + "/" + _clientId;
    QString reply = httpPost(url, "");
    _roomId = "";
    _clientId = "";

    destoryPeerConnection1();

    _initiator = false;
    _connected = false;

    //if (_pc)
    //{
    //    destoryPeerConnection(_pc);
    //    _pc = NULL;
    //}
}

void CallClient::rejoinRoom()
{
    qDebug() << "rejoinRoom";
    createPeerConnection1();
}

void CallClient::sendMessage(QString message)
{
    if (!inRoom()) return;
    if (!_initiator)
    {
        sendSignalingMessage(message);
    }
    else
    {
        QString url = _roomServer + "/message/" + _roomId + "/" + _clientId;
        QString reply = httpPost(url, message);
    }
}

void CallClient::sendSignalingMessage(QString message)
{
    if (!inRoom()) return;
    _signaling->sendMessage(message);
}

void CallClient::onReceiveSignalingMessage(QString message)
{
    qDebug() << "CallClient::onReceiveSignalingMessage: " << message;
    QJsonObject jobj = getJsonObjectFromString(message);
    if (!jobj.contains("type"))
    {

    }
    else if (jobj["type"] == "bye")
    {
        _initiator = true;
        _connected = false;
        emit hungup();
        //destoryPeerConnection1();
        //createPeerConnection1();
        //QString roomId = _roomId;
        //leaveRoom();
        //joinRoom(roomId);
    }
    else
    {
        handleSignalMessage(_pc, message.toStdString().c_str());
        _connected = true;
    }
}

QString CallClient::httpPost(QString url, QString content)
{
    qDebug() << url;
    qDebug() << "C->GAE: " << content;
    QNetworkRequest request;
    QByteArray data = content.toLocal8Bit();
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.post(request, data);
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    if (reply->error() != QNetworkReply::NoError)
    {
        return "";
    }

    QByteArray replyData = reply->readAll();
    QString replayStr = QString::fromLocal8Bit(replyData);
    qDebug() << url;
    qDebug() << "GAE->C: " << replayStr;
    return replayStr;
}