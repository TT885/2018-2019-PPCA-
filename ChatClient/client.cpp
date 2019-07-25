#include "client.h"

Client::Client(QObject* pa):QObject(pa)
{
    sock=new QTcpSocket(this);
    connect(sock,SIGNAL(readyRead()),this,SLOT(receiveMsg()));
}
Client::~Client() //发送下线消息
{
    QString msg("exit");
    sock->write(msg.toUtf8());
    sock->waitForBytesWritten();
    if(sock!=nullptr)
        delete sock;
}
void Client::ConnectToServer(QString ip,uint16_t port)
{
    sock->connectToHost(ip,port);
}

void Client::receiveMsg()
{
    QString msg=(sock->readAll());
    emit NewMsgCome(msg);
}

void Client::GetFeedback(QString &feedback)
{
    sock->waitForReadyRead();
    feedback=sock->readAll();
}
void Client::GetFeedback(QByteArray& feedback)
{
    sock->waitForReadyRead();
    feedback=sock->readAll();
}


void Client::SendMsgToServer(QString msg)
{
    sock->write(msg.toUtf8()); //?
    sock->waitForBytesWritten();  //??
}
void Client::SendFileToServer(QByteArray arr)
{
    sock->write(arr); //?
    sock->waitForBytesWritten();  //??
}

void Client::ChangeConnection(bool ch=0)
{
    if(ch)
        connect(sock,SIGNAL(readyRead()),this,SLOT(receiveMsg()));
    else
        disconnect(sock,SIGNAL(readyRead()),this,SLOT(receiveMsg()));
}
