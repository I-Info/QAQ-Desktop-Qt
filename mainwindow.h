#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString serverIp;
    qint16 serverPort;
    QString userName;

private slots:
    void on_connectionButton_clicked();

private:
    Ui::MainWindow *ui;
    QMessageBox msgBox;
    void errorBox(QString title = "Error!", QString text = "Please check the info you have inputed.");
};
#endif // MAINWINDOW_H
