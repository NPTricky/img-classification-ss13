#include "stdafx.h"
#include "FileExplorerListProxyModel.h"

FileExplorerListProxyModel::FileExplorerListProxyModel(QObject *parent /*= nullptr*/)
    : QSortFilterProxyModel(parent)
{
    connect(this,SIGNAL(modelReset()),this,SLOT(onSourceModelChanged()));

    setFilterRegExp(QRegExp("(.bmp|.dib|.jpeg|.jpg|.jpe|.jp2|.png|.pbm|.pgm|.ppm|.tiff|.tif)\\b", Qt::CaseInsensitive));
}

FileExplorerListProxyModel::~FileExplorerListProxyModel()
{

}

//bool FileExplorerListProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
//{
//    return true;
//}

void FileExplorerListProxyModel::onSourceModelChanged()
{
    m_FileSystemModel = qobject_cast<QFileSystemModel*>(sourceModel());
    invalidate();
}
