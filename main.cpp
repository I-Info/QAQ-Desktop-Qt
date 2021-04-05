#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    /*
     * QAQ Version: 1.0.2
     *
     * Source code Copyright (C) I-Info.
     * Contact: https://www.1inf0.com/
     *
     * Logo copyright belongs to ZJUT-JingHong
     *
     */

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
