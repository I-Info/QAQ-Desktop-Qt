#include "socketservice.h"

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
    this->serverIp = ip;
    this->serverPort = port;
    tcpSocket = new QTcpSocket();
    isConnected = false;
    isErrorOccurred = false;
    //Link
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(onReadMsg()));
}

void SocketService::stopSocket()
{
    tcpSocket->disconnectFromHost();
    tcpSocket->deleteLater();
}



void SocketService::onConnected()
{

}

void SocketService::onDisconnected()
{

}

void SocketService::onReadMsg()
{

}



void SocketService::socketConnect()
{
    int tryTimes = 0;
    while (!isConnected && !isErrorOccurred && tryTimes < 3) {
        tcpSocket->connectToHost(this->serverIp,this->serverPort);
        isConnected = tcpSocket->waitForConnected();
        tryTimes++;
    }
}
