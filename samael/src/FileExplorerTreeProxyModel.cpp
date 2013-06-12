#include "stdafx.h"
#include "FileExplorerTreeProxyModel.h"

FileExplorerTreeProxyModel::FileExplorerTreeProxyModel(QObject *parent /*= nullptr*/)
    : QSortFilterProxyModel(parent)
{
    connect(this,SIGNAL(modelReset()),this,SLOT(onSourceModelChanged()));
}

FileExplorerTreeProxyModel::~FileExplorerTreeProxyModel()
{

}

bool FileExplorerTreeProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = m_FileSystemModel->index(source_row, 0, source_parent);

    if (m_FileSystemModel->isDir(index)) ///< only accept directories
        return true;

    return false;
}

bool FileExplorerTreeProxyModel::filterAcceptsColumn( int source_column, const QModelIndex &source_parent ) const
{
    /// QFileSystemModel
    /// Column 0 = "Name"
    /// Column 1 = "Size"
    /// Column 2 = "Type"
    /// Column 3 = "Date Modified"
    return (source_column == 0);
}

void FileExplorerTreeProxyModel::onSourceModelChanged()
{
    m_FileSystemModel = qobject_cast<QFileSystemModel*>(sourceModel());
    invalidate();
}
