#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{   
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,2);

    ui->textBox->append("<meta charset='UTF-8'>");

    //For debug
    ui->serverInfo->setText("127.0.0.1:8080");
    ui->userInfo->setText("test");


    statusBar = new QLabel("QAQ");
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
    connect(mainService,&SocketService::groupList,this,&MainWindow::onGetGroupList);
    connect(mainService,&SocketService::historyMsg,this,&MainWindow::onGetHistory);
    serviceThread.start();
    emit setSocket();

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
    //qDebug()<<"main: "<<QThread::currentThreadId();
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
                    emit startSocket(serverIp,serverPort,userName);

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

void MainWindow::onGetStatus(const QString& status)
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

void MainWindow::onRecvedMsg(const QString& group, const QString& user, const QString& date, const QString& msg)
{
    /*Get message*/
    if (group == currentGroup) {
        QString temp = "<p><span style='color: blue'>" + user + "</span>@<span style='color: green'>" + date + "</span>: " + msg + "</p>";
        ui->textBox->append(temp);
    }
}

void MainWindow::onErrorOccurred(const int& code)
{
    if (code == 1) {
        //get message error
        errorBox("Network error","Read remote message failed, please cheack your network.");
    } else if (code == 2) {
        errorBox("Network error","Send message failed, please cheack your network.");
    } else if (code == 3) {
        errorBox("Network error","Connection request send failed, please cheack your network.");
    } else if (code == 4) {
        errorBox("Network error","Get message history failed, please cheack your network.");
    } else if (code == 5) {
        errorBox("Network error","Get group list failed, please cheack your network.");
    }

}

void MainWindow::onGetGroupList(const QStringList& groupList)
{
    ui->groupList->clear();
    foreach (QString groupName, groupList) {
        QListWidgetItem *item = new QListWidgetItem(groupName);
        ui->groupList->addItem(item);
    }
}

void MainWindow::onGetHistory(const QString &group, const QStringList &users, const QStringList &dates, const QStringList &msgs)
{
    if (group == currentGroup) {
        int num = users.length();
        for (int index = 0;index < num; index++) {
            QString temp = "<p><span style='color: blue'>" + users[index] + "</span>@<span style='color: green'>" + dates[index] + "</span>: " + msgs[index] + "</p>";
            ui->textBox->append(temp);
        }
        //ui->textBox->append("<p style='color: grey'>------------------------------</p>");
    }
}

void MainWindow::errorBox(const QString& title, const QString& text)
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
            if (!currentGroup.isEmpty()) {
                QByteArray base64(message.toUtf8());
                emit sendMsg(2,currentGroup,base64.toBase64());
            } else {
                errorBox("Error","Sorry, you can't send message before you select a group");
            }
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
    aboutBox.setWindowTitle("About");
    aboutBox.setText("About");
    aboutBox.setInformativeText("QAQ Client V1.0.0:\n Developed by I_Info, Node Sans");
    aboutBox.setStandardButtons(QMessageBox::Ok);
    aboutBox.setDefaultButton(QMessageBox::Ok);
    aboutBox.setIcon(QMessageBox::Information);
    aboutBox.open();//void block thread.

}

void MainWindow::on_lineEdit_returnPressed()
{
    on_sendButton_clicked();
}

void MainWindow::on_getGroup_clicked()
{
    if (!ui->serverInfo->isEnabled()) {
        emit sendMsg(4);
    } else  {
        errorBox("Error","Please connect first");
    }
}

void MainWindow::on_groupList_itemDoubleClicked(QListWidgetItem *item)
{
    if (!ui->serverInfo->isEnabled()) {
        currentGroup = item->text();
        ui->textBox->clear();
        statusBar->setText("Group: " + currentGroup);
        emit sendMsg(3,currentGroup);
    }
}
