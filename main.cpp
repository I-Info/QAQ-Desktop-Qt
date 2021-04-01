#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    /*
     * QAQ Version: 1.0.2
     */

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
