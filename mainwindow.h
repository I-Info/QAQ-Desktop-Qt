#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QThread>
#include <QLabel>
#include <QListWidgetItem>
#include "socketservice.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  signals:
    void startSocket(const QString&,const int&,const QString&);
    void stopSocket();
    void setSocket();
    void sendMsg(const int&,const QString& = "",const QString& = "");

  private slots:
    void on_connectionButton_clicked();
    void on_sendButton_clicked();
    void onGetStatus(const QString&);
    void onConnnected();
    void onDisConned();
    void onRecvedMsg(const QString&,const QString&,const QString&,const QString&);
    void onErrorOccurred(const int&);
    void onGetGroupList(const QStringList&);
    void onGetHistory(const QString&, const QStringList&, const QStringList&, const QStringList&);

    void on_action_QAQ_triggered();

    void on_lineEdit_returnPressed();

    void on_getGroup_clicked();

    void on_groupList_itemDoubleClicked(QListWidgetItem *item);

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
    void errorBox(const QString& = "Error", const QString& = "Please check infomation");


};
#endif // MAINWINDOW_H
