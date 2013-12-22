#include "assetpacker.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AssetPacker w;
    w.show();

    return a.exec();
}
