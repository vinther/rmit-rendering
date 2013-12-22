#include <QtGui>


#include "filetreeitem.hpp"
#include "filetreemodel.hpp"

#include <QMimeDatabase>


FileTreeModel::FileTreeModel(const QVector<QDir>& sources, QObject* parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    rootData << "Filename" << "Size (kB)" << "Size (compressed, kB)" << "MIME";
    rootItem = new FileTreeItem(rootData);

    for (const auto& dir: sources)
    {
        parseDirectory(dir, rootItem);
    }
}

FileTreeModel::~FileTreeModel()
{
    delete rootItem;
}

QModelIndex FileTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FileTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FileTreeItem*>(parent.internalPointer());

    FileTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex FileTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    FileTreeItem *childItem = static_cast<FileTreeItem*>(index.internalPointer());
    FileTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int FileTreeModel::rowCount(const QModelIndex &parent) const
{
    FileTreeItem *parentItem;
     if (parent.column() > 0)
         return 0;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<FileTreeItem*>(parent.internalPointer());

     return parentItem->childCount();
}

int FileTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<FileTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

bool FileTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::CheckStateRole || !index.isValid())
        return false;

    FileTreeItem *item = static_cast<FileTreeItem*>(index.internalPointer());

    item->setChecked( value == Qt::Checked );

    emit dataChanged(index, this->index(item->childCount(), index.column(), index));

    return true;
}

QVariant FileTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    FileTreeItem *item = static_cast<FileTreeItem*>(index.internalPointer());

    if ( role == Qt::CheckStateRole && index.column() == 0 )
    {
        return static_cast< int >( item->getChecked() ? Qt::Checked : Qt::Unchecked );
    }

    if (role != Qt::DisplayRole)
        return QVariant();

    return item->data(index.column());
}

Qt::ItemFlags FileTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if ( index.column() == 0 )
        flags |= Qt::ItemIsUserCheckable;

    return flags;
}

QVariant FileTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

void FileTreeModel::parseDirectory(const QDir &dir, FileTreeItem *parent)
{
    QFileInfoList files = dir.entryInfoList((QDir::AllEntries | QDir::NoDotAndDotDot), QDir::Name | QDir::DirsFirst); //Gets the file information

    for(const QFileInfo &fi : files) {
        QString fileName = fi.absoluteFilePath();

        auto* item = new FileTreeItem(fi, db.mimeTypeForFile(fi.absoluteFilePath()), parent);

        parent->appendChild(item);

        if(fi.isDir()) parseDirectory(QDir(fileName), item);
        else {
        }
    }
}
