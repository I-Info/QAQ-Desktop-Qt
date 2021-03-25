#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QTcpSocket>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serverIp = "127.0.0.1";
    serverPort = 8081;
    userName = "QQ";
    ui->statusbar->addPermanentWidget(new QLabel("Thank you for using QAQ"));
    defaultSocket = new QTcpSocket;
}

MainWindow::~MainWindow()
{
    delete defaultSocket;
    delete ui;
}


void MainWindow::on_connectionButton_clicked()
{
    QLineEdit *line1 = ui->serverInfo;
    QLineEdit *line2 = ui->userInfo;
    if (line1->isEnabled()) {
        if (line1->text() != "" && line2->text() != "" && line2->text().length() < 20) {
            QString serverInfo = line1->text().trimmed();
            userName = line2->text().trimmed();
            int sIndex = serverInfo.lastIndexOf(':');
            if (sIndex != -1) {
                QString tempServerIp = serverInfo.mid(0,sIndex);
                int tempServerPort = serverInfo.mid(sIndex+1).toInt();
                QRegularExpression regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
                QRegularExpressionMatch match = regExp.match(tempServerIp);
                if (match.hasMatch() && tempServerPort >0 && tempServerPort <= 65535) {
                    qDebug() << "pass";
                    serverIp = tempServerIp;
                    serverPort = tempServerPort;
                    //connect
                    socketConnect();


                } else {
                    errorBox();
                }
            }
            else {
                errorBox();
            }
    //        qDebug() << serverIp << serverPort;
        }
        else {
            errorBox();
        }
    }
    else {
        //disconnect
        socketDisConnect();
    }
}

void MainWindow::errorBox(QString title, QString text)
{
    msgBox.setWindowTitle(title);
    msgBox.setText(title);
    msgBox.setInformativeText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void MainWindow::socketConnect()
{
       defaultSocket->connectToHost(serverIp,serverPort);
       if (!defaultSocket->waitForConnected()) {
           errorBox("Socket connect failed", "Connect time out");
           return;
       }
       ui->serverInfo->setEnabled(false);
       ui->userInfo->setEnabled(false);
       ui->connectionButton->setText("disconnect");
}

void MainWindow::socketDisConnect()
{
    defaultSocket->disconnectFromHost();
    if (!defaultSocket->waitForDisconnected()) {
        errorBox("Failed","Disconnect failed, please try again later.");
        return;
    }
    ui->serverInfo->setEnabled(true);
    ui->userInfo->setEnabled(true);
    ui->connectionButton->setText("connect");
}

