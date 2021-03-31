#include "socketservice.h"
#include <QThread>
#include <QtCore>
#include <QTimer>
SocketService::SocketService(QObject *parent) : QObject(parent)
{
    //qDebug()<<"cons: "<<QThread::currentThreadId();
    tcpSocket = nullptr;
    isConnected = false;
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
    tcpSocket->setReadBufferSize(4096);
    //isConnected = false;
    //Link
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(ReadMsg()));
    connect(tcpSocket,&QTcpSocket::bytesWritten,this,&SocketService::onBytesWitten);
}

void SocketService::sendMsg(const int& mode, const QString& arg1 = "", const QString& arg2 = "")
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
        inWritten = true;
        if (tcpSocket->write(data.toUtf8()) == -1) {
            emit error(3);//connect request send failed.
        }
        return;
    }
    else if (mode == 2 && !arg1.isEmpty() && !arg1.isEmpty()) {
        QString data = "msg&;send&;" + arg1 + "&;" + arg2;
        if (tcpSocket->write(data.toUtf8()) == -1) {
            emit error(2);//message send failed
        }
        return;
    }
    else if (mode == 3 && !arg1.isEmpty()) {
        QString data = "msg&;list&;" + arg1;
        if (tcpSocket->write(data.toUtf8()) == -1) {
            emit error(4);//message history request failed
        }
        return;
    }
    else if (mode == 4) {
        QString data = "group&;list";
        if (tcpSocket->write(data.toUtf8()) == -1) {
            emit error(5);//group list request failed.
        }
        return;
    }

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
    char recvMsg[4096] = {};
    QString data;
    int code = tcpSocket->readLine(recvMsg,4096);
    while (code > 0) {
        data = QString(recvMsg);
        if (!data.isEmpty()) {
            handle(data);
        }
        code = tcpSocket->readLine(recvMsg,4096);
    }
    if (code == -1) {
        emit error(1);//Cannot read from remote.
        return;
    }
    return;
}

void SocketService::onBytesWitten()
{
    //check data send
    inWritten = false;
}

void SocketService::handle(const QString& data)
{
    QStringList args = data.split("&;",Qt::SkipEmptyParts);
    if (args[0] == "message" && args.length() >= 5) {
        int numberOfMsg = (args.length() - 1) / 4;
        for (int index = 1 ; index < numberOfMsg * 4 + 1; index += 4) {
            QString group = args[index];
            QString user = args[index + 1];
            QString date = args[index + 2];
            QString msg = QByteArray::fromBase64(args[index + 3].toUtf8());
            emit recvedMsg(group, user, date, msg);
        }
    }
    else if (args[0] == "historyMessage" && args.length() >= 5) {
        int numberOfMsg = (args.length() - 2) / 3;
        QString group = args[1];
        QStringList users, dates, msgs;
        for (int index = 2; index < numberOfMsg * 3 + 2; index += 3) {
            users.append(args[index]);
            dates.append(args[index + 1]);
            msgs.append(QByteArray::fromBase64(args[index + 2].toUtf8()));
            emit historyMsg(group, users, dates, msgs);
        }
    }
    else if (args[0] == "group" && args.length()>=2) {
        args.removeFirst();
        emit groupList(args);
        emit connStatus("Get group list finished");
    }
}

void SocketService::waitMSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
        while( QTime::currentTime() < _Timer )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}



void SocketService::socketConnect(const QString& ip, const int& port, const QString& userName)
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
        emit connStatus("Connected!Loading...");
        this->sendMsg(1,userName);
        emit connected();

        waitMSec(500);//Wait for bytes written

        if (!inWritten) {
            this->sendMsg(4);
            emit connStatus("Request group list success");
        } else {
            emit connStatus("Cannot request group list");
        }

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
