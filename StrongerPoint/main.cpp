#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    int appValue = app.exec();
    qDebug() << "APP VALUE: " << appValue;
    return appValue;
}
