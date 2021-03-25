#include "socketservice.h"

SocketService::SocketService(QObject *parent) : QObject(parent)
{
    isConnected = false;
    isThreadStoped = false;
}

SocketService::~SocketService()
{
    delete tcpSocket;
}

void SocketService::startSocket(const QString &ip, const int &port)
{
    this->serverIp = ip;
    this->serverPort = port;
    tcpSocket = new QTcpSocket();
    isConnected = false;
    isThreadStoped = false;
    //Link
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(tcpSocket,SIGNAL(disConnected()),this,SLOT(onDisconnected()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(onReadMsg()));
    run();
}

void SocketService::stopSocket()
{
    tcpSocket->disconnectFromHost();
}

void SocketService::run()
{
    while (!isThreadStoped) {
        if (!isConnected) {
            tcpSocket->connectToHost(this->serverIp,this->serverPort);
            isConnected = tcpSocket->waitForConnected();

        }

    }
}
