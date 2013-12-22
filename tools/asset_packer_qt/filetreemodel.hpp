#ifndef FILETREEMODEL_HPP
#define FILETREEMODEL_HPP

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QDir>

#include <QStandardItemModel>
#include <QMimeDatabase>
#include <QAbstractItemModel>

#include "filetreeitem.hpp"

class FileTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    FileTreeModel(const QVector<QDir>& sources, QObject *parent = 0);
    ~FileTreeModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    FileTreeItem* rootItem;

    QMimeDatabase db;

    void parseDirectory(const QDir& dir, FileTreeItem* parent);
};

#endif
