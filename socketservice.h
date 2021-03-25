#ifndef SOCKETSERVICE_H
#define SOCKETSERVICE_H

#include <QObject>

class SocketService : public QObject
{
    Q_OBJECT
public:
    explicit SocketService(QObject *parent = nullptr);

signals:

};

#endif // SOCKETSERVICE_H
