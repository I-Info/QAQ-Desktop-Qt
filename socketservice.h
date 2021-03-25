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
    void startSocket(const QString& ip, const int& port);
    void stopSocket();

signals:
    void error();
    void remoteMsg();


private slots:
    void onConnected();
    void onDisconnected();
    void onReadMsg();


private:
    QTcpSocket *tcpSocket;
    bool isConnected;
    bool isThreadStoped;
    QString serverIp;
    int serverPort;
    void run();

};

#endif // SOCKETSERVICE_H
