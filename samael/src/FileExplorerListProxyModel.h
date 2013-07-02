#ifndef FILEEXPLORERLISTPROXYMODEL_H
#define FILEEXPLORERLISTPROXYMODEL_H

#include <QSortFilterProxyModel>

class FileExplorerListProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FileExplorerListProxyModel(QObject *parent = nullptr);
    ~FileExplorerListProxyModel();

private:
    QFileSystemModel* m_FileSystemModel;

private slots:
    void onSourceModelChanged();

    virtual bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

};

#endif // FILEEXPLORERLISTPROXYMODEL_H
