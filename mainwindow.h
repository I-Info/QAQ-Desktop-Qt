#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTcpSocket>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectionButton_clicked();

private:
    Ui::MainWindow *ui;
    QMessageBox msgBox;
    QString serverIp;
    qint16 serverPort;
    QString userName;
    void errorBox(QString title = "Error!", QString text = "Please check the info you have inputed.");
    void socketConnect();
    void socketDisConnect();
    QTcpSocket *defaultSocket;
};
#endif // MAINWINDOW_H
