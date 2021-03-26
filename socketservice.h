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
    void recvedMsg(QString);

public slots:
    void socketConnect();
    void socketDisConn();
    void setSocket(const QString& ip, const int& port);
    void sendMsg(QString);


private slots:
    void onDisconnected();
    void ReadMsg();


private:
    QTcpSocket *tcpSocket;
    bool isConnected;
    QString serverIp;
    int serverPort;


};

#endif // SOCKETSERVICE_H
