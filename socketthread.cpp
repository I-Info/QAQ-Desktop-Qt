#include "socketthread.h"

SocketThread::SocketThread(QThread *parent) : QThread(parent)
{

}

SocketThread::~SocketThread() {
    delete TcpSocket;
}
