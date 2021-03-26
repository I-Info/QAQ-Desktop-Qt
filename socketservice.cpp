#include "socketservice.h"
#include <QThread>
SocketService::SocketService(QObject *parent) : QObject(parent)
{
    isConnected = false;
    serverIp = "0.0.0.0";
    serverPort = 8080;

}

SocketService::~SocketService()
{
    delete tcpSocket;
}

void SocketService::setSocket(const QString &ip, const int &port)
{
    qDebug()<<"set: "<<QThread::currentThreadId();
    tcpSocket = new QTcpSocket();
    this->serverIp = ip;
    this->serverPort = port;
    isConnected = false;
    //Link
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(onReadMsg()));
}


void SocketService::onDisconnected()
{
    qDebug() << "disconnect: " << QThread::currentThreadId();
    emit disConnected();
    isConnected = false;
    QThread::msleep(10);
    emit connStatus("Disconnected.");
}

void SocketService::onReadMsg()
{

}



void SocketService::socketConnect()
{
    qDebug()<<"Connect: "<<QThread::currentThreadId();
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
    return;
}
