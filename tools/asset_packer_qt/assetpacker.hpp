#ifndef ASSETPACKER_HPP
#define ASSETPACKER_HPP

#include <QMainWindow>
#include <QDir>

#include <cry_terra/utilities/memory.hpp>

#include "filetreemodel.hpp"

namespace Ui {
class AssetPacker;
}

class AssetPacker : public QMainWindow
{
    Q_OBJECT

public:
    explicit AssetPacker(QWidget *parent = 0);
    ~AssetPacker();

public slots:
    void readSettings();
    void writeSettings();

protected:
     void closeEvent(QCloseEvent *event);

private:
    Ui::AssetPacker *ui;

    QDir workingDirectory;

    QVector<QDir> sources;

    FileTreeModel* model;
};

#endif // ASSETPACKER_HPP
