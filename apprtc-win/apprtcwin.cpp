#include "apprtcwin.h"
#include <QJsonObject>
#include <QNetworkRequest>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QtCore/qeventloop.h>
#include <QNetworkReply>

#include "WssSignalingChannel.h"

apprtcwin::apprtcwin(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //_signalingChannel = new WssSignalingChannel("ws://8.142.13.126:8089/ws", "");
    _client = new CallClient();
    _needRejoin = false;
    startTimer(100);
    ui.testBtn->setEnabled(true);
    ui.testBtn2->setEnabled(false);
}

void apprtcwin::timerEvent(QTimerEvent* event)
{
    //if (_needRejoin)
    //{
    //    _client->rejoinRoom();
    //    _needRejoin = false;
    //    connect(_client, &CallClient::localVideoFrame, ui.localView, &MyGlView::onVideoFrame);
    //    connect(_client, &CallClient::remoteVideoFrame, ui.remoteView, &MyGlView::onVideoFrame);
    //    connect(_client, &CallClient::hungup, this, &apprtcwin::onHungup);
    //}

    if (_needRejoin)
    {
        _needRejoin = false;
        leaveRoom();

        //_client->disconnect();
        //_client->leaveRoom();

        //QString roomId = ui.roomEdit->text();
        //_client->joinRoom(roomId);
        //connect(_client, &CallClient::localVideoFrame, ui.localView, &MyGlView::onVideoFrame);
        //connect(_client, &CallClient::remoteVideoFrame, ui.remoteView, &MyGlView::onVideoFrame);
        //connect(_client, &CallClient::hungup, this, &apprtcwin::onHungup, Qt::QueuedConnection);
        //Qt::AutoConnection
    }
}

void apprtcwin::closeEvent(QCloseEvent* event)
{
    //event->ignore();
    if (_client)
    {
        _client->disconnect();
        delete _client;
        _client = NULL;
    }
}

apprtcwin::~apprtcwin()
{
    
}

void apprtcwin::joinRoom(QString roomId)
{
    ui.testBtn->setEnabled(false);
    ui.testBtn2->setEnabled(true);
    _client->joinRoom(roomId);
    connect(_client, &CallClient::localVideoFrame, ui.localView, &MyGlView::onVideoFrame);
    connect(_client, &CallClient::remoteVideoFrame, ui.remoteView, &MyGlView::onVideoFrame);
    connect(_client, &CallClient::hungup, this, &apprtcwin::onHungup);
}

void apprtcwin::leaveRoom()
{
    ui.testBtn->setEnabled(true);
    ui.testBtn2->setEnabled(false);
    _client->disconnect();
    _client->leaveRoom();
}

void apprtcwin::on_testBtn_clicked()
{
    QString roomId = ui.roomEdit->text();
    joinRoom(roomId);
}

void apprtcwin::onHungup()
{
    //_client->rejoinRoom();
    _client->disconnect();
    _needRejoin = true;
    //leaveRoom();
}

void apprtcwin::on_testBtn2_clicked()
{
    leaveRoom();
}

void apprtcwin::on_testBtn3_clicked()
{
    QString msg = ui.msgEdit->text();
    _client->sendMessage(msg);
}

void apprtcwin::on_testBtn4_clicked()
{
    QString msg = ui.msgEdit->text();
    _client->sendSignalingMessage(msg);
}
