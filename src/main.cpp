#include "mainwindow.h"

#include <QApplication>
#include <Log.h>
#include <InfoVersion.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//初始化日志函数 Log::setup(argv, APPNAME, 0~4);
#ifndef RELEASE_BUILD_NO_LOG
    Log::setup(argv, APPNAME, 4);
#endif

    MainWindow w;
    w.setWindowTitle(QString("%1-v%2 (Jackfahdin)")
                         .arg(APPNAME, APPLICATION_VERSION));

    w.show();
    return a.exec();
}
