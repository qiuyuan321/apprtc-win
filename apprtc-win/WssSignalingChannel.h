#pragma once
#include <QString>
#include <QWebSocket>
#include <QVector>

class SignalingChannel : public QObject
{
	Q_OBJECT
public:
	SignalingChannel();
	SignalingChannel(QString roomId, QString clientId);
	bool isConnected() { return _connected; }
	virtual void open() = 0;
	virtual void close() = 0;
	void sendMessage(QString message);

public slots:
	void onConnected();
	void onDisconnected();
	void onTextMessageReceived(QString message);

signals:
	void textMessageReceived(QString message);

private:
	void registerClient();
	virtual void send(QString message) = 0;
private:
	bool _connected;
	bool _registered;
	QString _roomId;
	QString _clientId;
	QVector<QString> _sdpMessages;
};

class WssSignalingChannel : public SignalingChannel
{
public:
	WssSignalingChannel(QString wssUrl, QString wssPostUrl, QString roomId, QString clientId);
	~WssSignalingChannel();

	virtual void open() override;
	virtual void close() override;
public slots:
	//void onConnected();
	//void onDisconnected();
private:
	virtual void send(QString message) override;

private:
	QString _wssUrl;
	QString _wssPostUrl;
	QWebSocket* _websocket;
};