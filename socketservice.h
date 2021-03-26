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
    void error(int errCode);
    void connStatus(QString);
    void connected();
    void disConnected();
    void remoteMsg(QString);

public slots:
    void socketConnect();
    void socketDisConn();
    void setSocket(const QString& ip, const int& port);


private slots:
    void onDisconnected();
    void onReadMsg();


private:
    QTcpSocket *tcpSocket;
    bool isConnected;
    QString serverIp;
    int serverPort;


};

#endif // SOCKETSERVICE_H
