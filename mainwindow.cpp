#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serverIp = "0.0.0.0";
    serverPort = 8081;
    userName = "QQ";


    //For debug
    ui->serverInfo->setText("0.0.0.0:8080");
    ui->userInfo->setText("test");


    statusBar = new QLabel("Thank you for using QAQ");
    ui->statusbar->addPermanentWidget(statusBar);

    //Initialize main socket service
    mainService = new SocketService();
    mainService->moveToThread(&serviceThread);

    connect(this,&MainWindow::setSocket,mainService,&SocketService::setSocket);
    connect(this,&MainWindow::startSocket,mainService,&SocketService::socketConnect);
    connect(this,&MainWindow::stopSocket,mainService,&SocketService::socketDisConn);
    connect(this,&MainWindow::sendMsg,mainService,&SocketService::sendMsg);
    connect(mainService,&SocketService::connStatus,this,&MainWindow::onGetStatus);
    connect(mainService,&SocketService::connected,this,&MainWindow::onConnnected);
    connect(mainService,&SocketService::disConnected,this,&MainWindow::onDisConned);
    connect(mainService,&SocketService::recvedMsg,this,&MainWindow::onRecvedMsg);
    connect(mainService,&SocketService::error,this,&MainWindow::onErrorOccurred);

    serviceThread.start();
}

MainWindow::~MainWindow()
{
    serviceThread.quit();
    serviceThread.wait();
    delete statusBar;
    delete mainService;
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
                    serverIp = tempServerIp;
                    serverPort = tempServerPort;
                    //connect
                    emit setSocket(serverIp, serverPort);
                    emit startSocket();

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
        emit stopSocket();

    }
}

void MainWindow::onGetStatus(QString status)
{
    statusBar->setText(status);
}

void MainWindow::onConnnected()
{
    ui->serverInfo->setEnabled(false);
    ui->userInfo->setEnabled(false);
    ui->connectionButton->setText("disconnect");
}

void MainWindow::onDisConned()
{
    ui->serverInfo->setEnabled(true);
    ui->userInfo->setEnabled(true);
    ui->connectionButton->setText("connect");
}

void MainWindow::onRecvedMsg(QString recvMsg)
{
    QString messages = ui->textBox->toPlainText();
    messages = messages + recvMsg + "\n";
    ui->textBox->setText(messages);
}

void MainWindow::onErrorOccurred(int code)
{
    if (code == 1) {
        //get message error
        errorBox("Network error","Get remote message failed, cheack your network connection.");
        return;
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
    msgBox.open();//void block thread.
}


void MainWindow::on_sendButton_clicked()
{
    QString message = ui->lineEdit->text();
    if (!message.isEmpty() && !ui->serverInfo->isEnabled()) {
        if (message.length() < 450) {
            emit sendMsg(message);
            ui->lineEdit->setText("");
        } else {
            errorBox("Error", "the message is too long.");
        }
    }
    else if (ui->serverInfo->isEnabled()) {
        errorBox("Error", "Sorry, you can't send message before you connect to remote server");
        ui->lineEdit->setText(NULL);
    }
    return;
}

void MainWindow::on_action_QAQ_triggered()
{

}

void MainWindow::on_lineEdit_returnPressed()
{
    on_sendButton_clicked();
}
