#ifndef FILEEXPLORERTREEPROXYMODEL_H
#define FILEEXPLORERTREEPROXYMODEL_H

#include <QSortFilterProxyModel>

class FileExplorerTreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FileExplorerTreeProxyModel(QObject *parent = nullptr);
    ~FileExplorerTreeProxyModel();

    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    QFileSystemModel* m_FileSystemModel;

private slots:
    void onSourceModelChanged();

    virtual bool filterAcceptsColumn( int source_column, const QModelIndex &source_parent ) const;

};

#endif // FILEEXPLORERTREEPROXYMODEL_H
