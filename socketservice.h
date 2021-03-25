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
    void setSocket(const QString& ip, const int& port);

signals:
    void error(int errCode);
    void connStatus(QString);
    void remoteMsg(QString);

public slots:
    void socketConnect();
    void socketDisConn();


private slots:
    void onConnected();
    void onDisconnected();
    void onReadMsg();


private:
    QTcpSocket *tcpSocket;
    bool isConnected;
    bool isErrorOccurred;
    QString serverIp;
    int serverPort;


};

#endif // SOCKETSERVICE_H
