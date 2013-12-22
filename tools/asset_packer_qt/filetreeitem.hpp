#ifndef FILETREEITEM_HPP
#define FILETREEITEM_HPP

#include <QList>
#include <QVariant>
#include <QVector>
#include <QDir>
#include <QMimeType>

#include <QStandardItem>

class FileTreeItem
{

public:
    explicit FileTreeItem(const QFileInfo &fileInfo, QMimeType mimeType, FileTreeItem *parent = 0);
    explicit FileTreeItem(const QVector<QVariant>& data, FileTreeItem *parent = 0);
    ~FileTreeItem();

    void appendChild(FileTreeItem* child);
    FileTreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    FileTreeItem *parent();
    int row() const;

    bool getChecked() const;
    void setChecked(bool value);

    void compress();

private:
    QVector<FileTreeItem*> childItems;
    QVector<QVariant> itemData;
    FileTreeItem *parentItem;

    QFileInfo fileInfo;
    QMimeType mimeType;

    QVector<unsigned char> compressedData;

    size_t size_before, size_after;

    bool checked;
};


#endif // FILETREEITEM_HPP
