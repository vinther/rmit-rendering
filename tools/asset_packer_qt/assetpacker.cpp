#include <QSettings>
#include <QDebug>
#include <QFileDialog>
#include <QCommandLineParser>


#include "assetpacker.hpp"
#include "filetreemodel.hpp"

#include "ui_assetpacker.h"



AssetPacker::AssetPacker(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AssetPacker)
{
    QCoreApplication::setOrganizationName("Cry Terra");
    QCoreApplication::setApplicationName("Asset Packer");

    ui->setupUi(this);

    QVector<QDir> sources;

    sources << (QDir("/home/svp/cry/assets"));

    model = new FileTreeModel(sources, this);


    QModelIndex parentIndex = model->index(0, 0);
    int numRows = model->rowCount(parentIndex);
    for (int row = 0; row < numRows; ++row) {
            QModelIndex index = model->index(row, 0, parentIndex);
            qDebug() << model->data(index, Qt::DisplayRole).toString();

         }

    ui->treeView->setModel(model);
    ui->treeView->show();

    readSettings();
}

AssetPacker::~AssetPacker()
{
    delete ui;
}


void AssetPacker::readSettings()
{
    QSettings settings;

    move(settings.value("mainWindow/position", pos()).toPoint());
    resize(settings.value("mainWindow/size", size()).toSize());

    workingDirectory = settings.value("workingDirectory", QDir::currentPath()).toString();
}

void AssetPacker::writeSettings()
{
    QSettings settings;

    settings.setValue("mainWindow/position", pos());
    settings.setValue("mainWindow/size", size());
    settings.setValue("workingDirectory", workingDirectory.path());
}

void AssetPacker::closeEvent(QCloseEvent*)
{
    writeSettings();
}
