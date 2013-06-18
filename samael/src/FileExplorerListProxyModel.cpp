#include "stdafx.h"
#include "FileExplorerListProxyModel.h"

FileExplorerListProxyModel::FileExplorerListProxyModel(QObject *parent /*= nullptr*/)
    : QSortFilterProxyModel(parent)
{
    connect(this,SIGNAL(modelReset()),this,SLOT(onSourceModelChanged()));

    QRegExp m_Filters = QRegExp("(.bmp|.dib|.jpeg|.jpg|.jpe|.jp2|.png|.pbm|.pgm|.ppm|.tiff|.tif)\\b", Qt::CaseInsensitive);
    setFilterRegExp(m_Filters);
    
    ///! BUG: empty indices after conversion between proxy models
    ///! test
    ///! 
}

FileExplorerListProxyModel::~FileExplorerListProxyModel()
{

}

bool FileExplorerListProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    //QModelIndex index = m_FileSystemModel->index(source_row, 0, source_parent);

    //if (m_FileSystemModel->isDir(index)) ///< only accept directories
    //    return false;

    return true;
}

void FileExplorerListProxyModel::onSourceModelChanged()
{
    m_FileSystemModel = qobject_cast<QFileSystemModel*>(sourceModel());
    invalidate();
}
