#include "filetreeitem.hpp"

#include <QDebug>

#include <lzo/lzo1x.h>

FileTreeItem::FileTreeItem(const QFileInfo &fileInfo, QMimeType mimeType, FileTreeItem *parent)
    : itemData(5), parentItem(parent), fileInfo(fileInfo), mimeType(mimeType), checked(false)
{
    lzo_init();

    if (!fileInfo.isDir())
    {
        QFile file(fileInfo.absoluteFilePath());

        file.open(QFile::ReadOnly);

        compressedData.resize(file.size() + (file.size() / 16) + 64 + 3);

        QVector<unsigned char> blob(file.size());
        QVector<unsigned char> wrkmem(LZO1X_999_MEM_COMPRESS, Qt::Uninitialized);

        file.read((char*) blob.data(), file.size());

        size_t out_len = compressedData.size();
        //lzo1x_999_compress((const lzo_bytep) blob.data(), blob.size(), (lzo_bytep) compressedData.data(), &out_len, wrkmem.data());

        compressedData.resize(out_len);
    }
}

FileTreeItem::FileTreeItem(const QVector<QVariant> &data, FileTreeItem *parent)
    : itemData(data), parentItem(parent)
{
}

FileTreeItem::~FileTreeItem()
{
    qDeleteAll(childItems);
}

FileTreeItem *FileTreeItem::child(int number)
{
    return childItems.value(number);
}

int FileTreeItem::childCount() const
{
    return childItems.count();
}

int FileTreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<FileTreeItem*>(this));

    return 0;
}

bool FileTreeItem::getChecked() const
{
    return checked;
}

void FileTreeItem::setChecked(bool value)
{
    checked = value;

    for (auto* child: childItems)
        child->setChecked(checked);
}


int FileTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant FileTreeItem::data(int column) const
{
    if (!parentItem)
    {
        return itemData.at(column);
    }

    switch (column)
    {
    case 0: return fileInfo.fileName(); break;
    case 1:
        if (fileInfo.isDir())
            return QString();
        else
            return (fileInfo.size() / 1024);
        break;
    case 2:
        if (fileInfo.isDir())
            return QString();
        else
            return (compressedData.size() / 1024);
        break;

    case 3: return mimeType.name(); break;
    default: return QVariant();
    }
}

FileTreeItem *FileTreeItem::parent()
{
    return parentItem;
}

void FileTreeItem::appendChild(FileTreeItem *item)
{
    childItems.append(item);
}
