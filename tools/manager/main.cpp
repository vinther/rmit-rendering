#include "mainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Cry Terra");
    QCoreApplication::setApplicationName("Tool Manager");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
