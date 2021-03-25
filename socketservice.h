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
    void stopSocket();

signals:
    void error(QString msg);
    void connStatus();
    void remoteMsg();

public slots:
    void socketConnect();


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
