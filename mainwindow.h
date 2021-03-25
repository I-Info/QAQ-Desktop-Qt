#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QThread>
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

private slots:
    void on_connectionButton_clicked();

private:
    Ui::MainWindow *ui;
    QMessageBox msgBox;
    QString serverIp;
    qint16 serverPort;
    QString userName;
    SocketService *mainService;
    QThread serviceThread;
    void errorBox(QString title = "Error!", QString text = "Please check the info you have inputed.");

};
#endif // MAINWINDOW_H
