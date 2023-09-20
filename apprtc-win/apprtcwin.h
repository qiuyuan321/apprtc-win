#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_apprtcwin.h"

#include "WssSignalingChannel.h"
#include "CallClient.h"

class apprtcwin : public QMainWindow
{
    Q_OBJECT

public:
    apprtcwin(QWidget *parent = Q_NULLPTR);
    ~apprtcwin();
protected:
    void closeEvent(QCloseEvent* event);
    void timerEvent(QTimerEvent* event);
    void joinRoom(QString roomId);
    void leaveRoom();

public slots:
    void on_testBtn_clicked();
    void on_testBtn2_clicked();
    void on_testBtn3_clicked();
    void on_testBtn4_clicked();
    void onHungup();

private:
    Ui::apprtcwinClass ui;
    CallClient* _client;
    bool _needRejoin;
};
