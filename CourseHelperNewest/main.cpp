#include "mainwindow.h"
#include "test.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("PKU选课助手");
    w.resize(1000, 600);
    w.show();

    return a.exec();
}
