#include "socketservice.h"
SocketService::SocketService(QObject *parent) : QObject(parent)
{
    isConnected = false;
    serverIp = "0.0.0.0";
    serverPort = 8080;
    tcpSocket = new QTcpSocket();
}

SocketService::~SocketService()
{
    delete tcpSocket;
}

void SocketService::setSocket(const QString &ip, const int &port)
{
    this->serverIp = ip;
    this->serverPort = port;
    isConnected = false;
    isErrorOccurred = false;
    //Link
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(onReadMsg()));
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
        tcpSocket->connectToHost(serverIp,serverPort);
        isConnected = tcpSocket->waitForConnected(3000);
        std::string status = "Connecting.." + std::to_string(tryTimes);
        //emit connStatus(QString().fromStdString(status));
        tryTimes++;
    }
    if (tryTimes >= 3) {
        //emit error(0);//0 = CONN_TIME_OUT
    }
    return;
}

void SocketService::socketDisConn()
{
    tcpSocket->disconnectFromHost();
    return;
}
