#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QTcpSocket>

class SocketThread : public QThread
{
    Q_OBJECT

public:
    explicit SocketThread(QThread *parent = nullptr);
    ~SocketThread();

    void startThread(const QString& ip, int port);
    void stopThread();

Q_SIGNALS:
    void errors(QString err);
    void msg(QString msg);

private slots:
    void onConnect();
    void onDisconnect();
    void onReadMsg();

private:
    QTcpSocket *TcpSocket;
    QString serverIp;
    int serverPort;
    bool isThreadStopped;
    bool isConnected;


};

#endif // SOCKETTHREAD_H
