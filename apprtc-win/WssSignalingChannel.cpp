#include "WssSignalingChannel.h"
#include "JsonUtils.h"
#include <QNetworkRequest>
#include <qnetworkrequest.h>

SignalingChannel::SignalingChannel():
	_connected(false), _registered(false), _roomId(""), _clientId("")
{

}

SignalingChannel::SignalingChannel(QString roomId, QString clientId) :
	_connected(false), _registered(false), _roomId(roomId), _clientId(clientId)
{

}

void SignalingChannel::onConnected()
{
	qDebug() << "SignalingChannel::onConnected...";
	_connected = true;
	send("{}");
	registerClient();
	for (int i = 0; i < _sdpMessages.count(); i++)
	{
		sendMessage(_sdpMessages[i]);
	}
	_sdpMessages.clear();
}

void SignalingChannel::onDisconnected()
{
	qDebug() << "SignalingChannel::onDisconnected...";
	_connected = false;
	_registered = false;
}

void SignalingChannel::onTextMessageReceived(QString message)
{
	QJsonObject jobj = getJsonObjectFromString(message);
	QString errorStr = jobj.value("error").toString();
	if (!errorStr.isEmpty() || !jobj.contains("msg"))
	{
		qDebug() << "WSS->C: [error]" << message;
		return;
	}
	qDebug() << "WSS->C: " << message;
	emit textMessageReceived(jobj["msg"].toString());
}

void SignalingChannel::registerClient()
{
	if (!_connected) return;
	if (_registered) return;

	QJsonObject jobj;
	jobj["cmd"] = "register";
	jobj["roomid"] = _roomId;
	jobj["clientid"] = _clientId;

	send(getStringFromJsonObject(jobj));
	_registered = true;
}

void SignalingChannel::sendMessage(QString message)
{
	if (!_connected)
	{
		_sdpMessages.push_back(message);
		return;
	}

	QJsonObject jobj;
	jobj["cmd"] = "send";
	jobj["msg"] = message;

	send(getStringFromJsonObject(jobj));
}

WssSignalingChannel::WssSignalingChannel(QString wssUrl, QString wssPostUrl, QString roomId, QString clientId)
	: _wssUrl(wssUrl), _wssPostUrl(wssPostUrl), SignalingChannel(roomId, clientId)
{
	_websocket = new QWebSocket();
	connect(_websocket, &QWebSocket::connected, this, &SignalingChannel::onConnected);
	connect(_websocket, &QWebSocket::disconnected, this, &SignalingChannel::onDisconnected);
	connect(_websocket, &QWebSocket::textMessageReceived, this, &SignalingChannel::onTextMessageReceived);
}

WssSignalingChannel::~WssSignalingChannel()
{
	delete _websocket;
}

void WssSignalingChannel::open()
{
	//_websocket->setMaskGenerator(new QMaskGenerator());
	//_websocket->open(QUrl(_wssUrl));
	QNetworkRequest request;
	request.setUrl(_wssUrl);
	//request.setRawHeader("Sec-WebSocket-Protocol", "");
	request.setRawHeader("Origin", "http://qttest");
	//request.setRawHeader("Sec-Websocket-Key", "GfJh36tC10M7GZaBvtUbAA == ");
	//request.setRawHeader("User-Agent", "Mozilla / 5.0 (Windows NT 10.0; Win64; x64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 116.0.0.0 Safari / 537.36");
	//request.setRawHeader("Sec-Websocket-Extensions", "permessage-deflate; client_max_window_bits");
	_websocket->open(request);
}

void WssSignalingChannel::close()
{
	_websocket->close();
}

void WssSignalingChannel::send(QString message)
{
	_websocket->sendTextMessage(message);
	qDebug() << "C->WSS: " << message;
}

