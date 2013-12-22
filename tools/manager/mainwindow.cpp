#include <QSettings>
#include <QDebug>
#include <QFileDialog>


#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    readSettings();

    ui->launchAssetPackerButton->setToolDescription(tr("Launch the asset packer tool"));
    ui->launchFilePathHasherButton->setToolDescription(tr("Launch the file path hasher tool"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSettings()
{
    QSettings settings;

    move(settings.value("mainWindow/position", pos()).toPoint());
    resize(settings.value("mainWindow/size", size()).toSize());

    setWorkingDirectory(settings.value("workingDirectory", QDir::currentPath()).toString());
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.setValue("mainWindow/position", pos());
    settings.setValue("mainWindow/size", size());
    settings.setValue("workingDirectory", workingDirectory.path());
}

void MainWindow::openWorkingDirectoryPrompt()
{
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::Directory);

    qDebug() << workingDirectory;

    dialog.setDirectory(workingDirectory);

    QStringList fileNames;
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();

        if (fileNames.size() == 1)
            setWorkingDirectory(fileNames[0]);
    }

}

void MainWindow::setWorkingDirectory(QString dir)
{
    workingDirectory = QDir(dir);

    emit workingDirectoryChanged(dir);
}

void MainWindow::closeEvent(QCloseEvent*)
{
    writeSettings();
}



