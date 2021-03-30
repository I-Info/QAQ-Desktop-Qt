#include "socketservice.h"
#include <QThread>
SocketService::SocketService(QObject *parent) : QObject(parent)
{
    //qDebug()<<"cons: "<<QThread::currentThreadId();
    tcpSocket = nullptr;
    isConnected = false;
    serverIp = "0.0.0.0";
    serverPort = 8080;
}

SocketService::~SocketService()
{
    if (tcpSocket != nullptr) {
        delete tcpSocket;
    }
}

void SocketService::setSocket()
{
    //qDebug()<<"set: "<<QThread::currentThreadId();
    tcpSocket = new QTcpSocket();
    //isConnected = false;
    //Link
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(ReadMsg()));
}

void SocketService::sendMsg(int mode, QString arg1 = "", QString arg2 = "")
{
    /*modes:
     * user connect [name] 1
     * user status 0
     * msg send [group_name] [msg] 2
     * msg list [group_name] 3 -> message {[group_name] [username] [date] [msg]}
     * group list 4-> group {[group_name]}
     *
     * */
    if (mode == 1 && !arg1.isEmpty()) {
        QString data = "user&;connect&;" + arg1;
        if (tcpSocket->write(data.toLatin1()) == -1) {
            emit error(2);
        }
        return;
    }
    else if (mode == 2 && !arg1.isEmpty() && !arg1.isEmpty()) {
        QString data = "msg&;send&;" + arg1 + "&;" + arg2;
        if (tcpSocket->write(data.toLatin1()) == -1) {
            emit error(2);
        }
        return;
    }
    else if (mode == 3 && !arg1.isEmpty()) {
        QString data = "msg&;list&;" + arg1;
        if (tcpSocket->write(data.toLatin1()) == -1) {
            emit error(2);
        }
        return;
    }
    else if (mode == 4) {
        QString data = "group&;list";
        if (tcpSocket->write(data.toLatin1()) == -1) {
            emit error(2);
        }
        return;
    }


//    if (!message.isEmpty()) {
//        message = "msg&;send&;" + message;
//        int code = tcpSocket->write(message.toLatin1());
//        if (code == -1) {
//            emit error(2);//Cannot write to remote.
//        }
//        return;
//    }
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
    QString data;
    int code = tcpSocket->read(recvMsg,512);
    if (code == -1) {
        emit error(1);//Cannot read from remote.
        return;
    }
    data = QString(recvMsg);
    if (!data.isEmpty()) {
        handle(data);
    }
    return;
}

void SocketService::handle(QString data)
{
    QStringList args = data.split("&;",Qt::SkipEmptyParts);
    if (args[0] == "message" && args.length() >= 4) {
        int numberOfMsg = (args.length() - 1) / 3;
        for (int index = 1 ; index < numberOfMsg * 3 +1; index += 3) {
            QString user = args[index];
            QString date = args[index + 1];
            QString msg = args[index + 2];
            emit recvedMsg(user, date, msg);
        }

    }
}



void SocketService::socketConnect(QString ip,int port,QString userName)
{
    //qDebug()<<"Connect: "<<QThread::currentThreadId();
    this->serverIp = ip;
    this->serverPort = port;
    tcpSocket->connectToHost(serverIp,serverPort);
    emit connStatus("Connecting...");
    isConnected = tcpSocket->waitForConnected(5000);
    if (!isConnected) {
        emit connStatus("Connect failed");
    } else {
        emit connected();
        QThread::msleep(10);
        emit connStatus("Connected");
        this->sendMsg(1,userName);
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
