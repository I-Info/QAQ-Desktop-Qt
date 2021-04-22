#ifndef SOCKETSERVICE_H
#define SOCKETSERVICE_H

#include <QObject>
#include <QTcpSocket>

class QTcpSocket;

class SocketService : public QObject {
  Q_OBJECT
public:
  explicit SocketService(QObject *parent = nullptr);
  ~SocketService();

signals:
  void error(int);
  void connStatus(const QString &);
  void connected();
  void disConnected();
  void recvedMsg(const QString &, const QString &, const QString &,
                 const QString &);

  void historyMsg(const QString &, const QStringList &, const QStringList &,
                  const QStringList &);

  void groupList(const QStringList &);

public slots:
  void socketConnect(const QString &, const int &, const QString &);
  void socketDisConn();
  void setSocket();
  void sendMsg(const int &, const QString &, const QString &);

private slots:
  void onDisconnected();
  void ReadMsg();
  void onBytesWitten();

private:
  QTcpSocket *tcpSocket;
  bool isConnected;
  bool inWritten;
  QString serverIp;
  int serverPort;
  int groupMode;
  bool inReading;
  QString readCache;

  void handle(const QString &);
  void waitMSec(unsigned int);
  bool waitForWritten();
};

#endif // SOCKETSERVICE_H
