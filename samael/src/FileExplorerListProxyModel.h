#ifndef FILEEXPLORERLISTPROXYMODEL_H
#define FILEEXPLORERLISTPROXYMODEL_H

#include <QSortFilterProxyModel>

class FileExplorerListProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FileExplorerListProxyModel(QObject *parent = nullptr);
    ~FileExplorerListProxyModel();

    //virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    QFileSystemModel* m_FileSystemModel;
    QRegExp m_Filters;

private slots:
    void onSourceModelChanged();

};

#endif // FILEEXPLORERLISTPROXYMODEL_H
