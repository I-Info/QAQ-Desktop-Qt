#include "socketservice.h"
#include <QThread>
SocketService::SocketService(QObject *parent) : QObject(parent)
{
    tcpSocket = nullptr;
    isConnected = false;
    serverIp = "0.0.0.0";
    serverPort = 8080;
}

SocketService::~SocketService()
{
    if (tcpSocket) {
        if (isConnected) {
            tcpSocket->disconnectFromHost();
        }
        delete tcpSocket;
    }
}

void SocketService::setSocket(const QString &ip, const int &port)
{
    //qDebug()<<"set: "<<QThread::currentThreadId();
    tcpSocket = new QTcpSocket();
    this->serverIp = ip;
    this->serverPort = port;
    isConnected = false;
    //Link
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(ReadMsg()));
}

void SocketService::sendMsg()
{
    //BLANK
}


void SocketService::onDisconnected()
{
    //qDebug() << "disconnect: " << QThread::currentThreadId();
    emit disConnected();
    isConnected = false;
    QThread::msleep(10);
    emit connStatus("Disconnected.");
}

void SocketService::ReadMsg()
{
    char recvMsg[512] = {};
    QString messages;
    int code = tcpSocket->readLine(recvMsg,512);
    while (code > 0) {
        messages += QString(recvMsg);
        code = tcpSocket->readLine(recvMsg,512);
    }
    if (code == -1) {
        emit error(1);//Cannot read from remote.
    }
    if (!messages.isNull()) {
        emit recvedMsg(messages);
    }
    return;
}



void SocketService::socketConnect()
{
    //qDebug()<<"Connect: "<<QThread::currentThreadId();
    tcpSocket->connectToHost(serverIp,serverPort);
    emit connStatus("Connecting...");
    isConnected = tcpSocket->waitForConnected(5000);
    if (!isConnected) {
        emit connStatus("Connect failed");
    } else {
        emit connected();
        QThread::msleep(10);
        emit connStatus("Connected");
    }
}

void SocketService::socketDisConn()
{
    tcpSocket->disconnectFromHost();
    if (isConnected) {
       tcpSocket->waitForDisconnected();
    }
    return;
}
