#pragma once
#include <QString>
#include <QObject>
#include "WssSignalingChannel.h"

class CallClient : public QObject
{
	Q_OBJECT
public:
	CallClient();
	~CallClient();
	void joinRoom(QString roomId);
	void leaveRoom();
	void rejoinRoom();
	void sendMessage(QString message);
	void sendSignalingMessage(QString message);
	void onLocalVideoFrame1(const char* buffer, int format, int width, int height);
	void onRemoteVideoFrame1(const char* buffer, int format, int width, int height);
public slots:
	void onReceiveSignalingMessage(QString message);
signals:
	void localVideoFrame(void* buffer, int width, int height, int type);
	void remoteVideoFrame(void* buffer, int width, int height, int type);
	void hungup();
private:
	void createPeerConnection1();
	void destoryPeerConnection1();
	bool inRoom();
	QString httpPost(QString path, QString content);

private:
	bool _showFlag;
	bool _connected;
	bool _initiator;
	QString _roomId;
	QString _clientId;
	QString _roomServer;
	SignalingChannel* _signaling;
	void* _pc;
};