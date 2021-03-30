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
    void startSocket(QString,int,QString);
    void stopSocket();
    void setSocket();
    void sendMsg(int,QString,QString);

private slots:
    void on_connectionButton_clicked();
    void on_sendButton_clicked();
    void onGetStatus(QString);
    void onConnnected();
    void onDisConned();
    void onRecvedMsg(QString,QString,QString,QString);
    void onErrorOccurred(int);

    void on_action_QAQ_triggered();

    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    QMessageBox msgBox;
    QMessageBox aboutBox;
    QString serverIp;
    qint16 serverPort;
    QString userName;
    QString currentGroup;
    SocketService *mainService;
    QThread serviceThread;
    QLabel *statusBar;
    void errorBox(QString title = "Error!", QString text = "Please check the info you have inputed.");


};
#endif // MAINWINDOW_H
