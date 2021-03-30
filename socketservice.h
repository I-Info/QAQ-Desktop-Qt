#ifndef SOCKETSERVICE_H
#define SOCKETSERVICE_H

#include <QObject>
#include <QTcpSocket>

class QTcpSocket;

class SocketService : public QObject
{
    Q_OBJECT
public:
    explicit SocketService(QObject *parent = nullptr);
    ~SocketService();

signals:
    void error(int);
    void connStatus(QString);
    void connected();
    void disConnected();
    void recvedMsg(QString, QString, QString);

public slots:
    void socketConnect(QString,int,QString);
    void socketDisConn();
    void setSocket();
    void sendMsg(int,QString,QString);


private slots:
    void onDisconnected();
    void ReadMsg();

private:
    QTcpSocket *tcpSocket;
    bool isConnected;
    QString serverIp;
    int serverPort;

    void handle(QString);

};

#endif // SOCKETSERVICE_H
