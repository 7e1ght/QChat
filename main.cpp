#include <QApplication>
#include <QDebug>
#include <QVector>

#include "crypt.h"
#include "mainwindow.h"
#include "passwordsharing.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
