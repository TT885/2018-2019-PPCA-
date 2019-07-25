#ifndef CLIENT_H
#define CLIENT_H

#include <QLabel>
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QThread>
#include <QFile>
#include <QFileDialog>
#include <QWidget>
#include <QDateTime>
#include <bits/stdc++.h>
class Client:public QObject
{
    Q_OBJECT
public:
    Client(QObject* pa);
    ~Client();
    void ConnectToServer(QString ip,uint16_t port);
    void SendMsgToServer(QString);
    void SendFileToServer(QByteArray arr);
    void GetFeedback(QString&);
    void GetFeedback(QByteArray&);
    void ChangeConnection(bool ch);
    QString name;
signals:
    void NewMsgCome(QString&);
public slots:
    void receiveMsg();
private:
    QTcpSocket *sock;
};

#endif // CLIENT_H
