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
    if (tcpSocket) {
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

void SocketService::sendMsg(int mode, QString arg1 = "", QString arg2 = "")
{
    /*modes:
     * user connect [name] 1
     * user status 0
     * user disconnect 2
     * msg send [group_name] [msg] 3
     * msg list [group_name] 4 -> message {[group_name] [username] [date] [msg]}
     * group list 5-> group {[group_name]}
     *
     * */
    if (mode == 1 && !arg1.isEmpty()) {
        QString data = "user&;connect&;" + arg1;
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
    tcpSocket->deleteLater();
    return;
}
