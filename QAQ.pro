QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += skip_target_version_ext

VERSION = 1.1.0
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    socketservice.cpp

HEADERS += \
    mainwindow.h \
    socketservice.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    QAQ_zh_CN.ts

# Static Builds
QTPREFIX=$$[QT_INSTALL_PREFIX]
equals(QTPREFIX, "/home/info/Documents/Qt"){
    message("--STATIC BUILD--")
    CONFIG += qt static
    QMAKE_LFLAGS += -static-libgcc -static-libstdc++
} else {
    message("--NON-STATIC BUILD--")
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = logo.ico

RESOURCES += \
    res.qrc
