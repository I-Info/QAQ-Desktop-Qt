#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QThread>
#include <QLabel>
#include "socketservice.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void startSocket();
    void stopSocket();
    void setSocket(const QString&, const int&);
    void sendMsg(QString);

private slots:
    void on_connectionButton_clicked();
    void on_sendButton_clicked();
    void onGetStatus(QString);
    void onConnnected();
    void onDisConned();
    void onRecvedMsg(QString);
    void onErrorOccurred(int);


private:
    Ui::MainWindow *ui;
    QMessageBox msgBox;
    QString serverIp;
    qint16 serverPort;
    QString userName;
    SocketService *mainService;
    QThread serviceThread;
    QLabel *statusBar;
    void errorBox(QString title = "Error!", QString text = "Please check the info you have inputed.");

};
#endif // MAINWINDOW_H
