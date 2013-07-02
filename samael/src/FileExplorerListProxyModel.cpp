#include "stdafx.h"
#include "FileExplorerListProxyModel.h"

FileExplorerListProxyModel::FileExplorerListProxyModel(QObject *parent /*= nullptr*/)
    : QSortFilterProxyModel(parent)
{
    connect(this,SIGNAL(modelReset()),this,SLOT(onSourceModelChanged()));

    QRegExp m_Filters = QRegExp("(bmp|dib|jpeg|jpg|jpe|jp2|png|pbm|pgm|ppm|tiff|tif)$", Qt::CaseInsensitive);
    
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

    //if (m_FileSystemModel->isDir(index))
    //    return false;

    return true;
}

void FileExplorerListProxyModel::onSourceModelChanged()
{
    m_FileSystemModel = qobject_cast<QFileSystemModel*>(sourceModel());
    invalidate();
}

bool FileExplorerListProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    return true;
}

QVariant FileExplorerListProxyModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole */) const
{
    QFileInfo info = m_FileSystemModel->fileInfo(mapToSource(index));

    switch (role)
    {
    //case (Qt::ToolTipRole):
    //    return QVariant("Test");
    //    break;
    //case (Qt::BackgroundColorRole):
    //    break;
    //    if (index.row() % 2 == 1) return QColor(qRgb(240,240,240));
    case (Qt::DisplayRole):
        return QVariant(); // filenames deactivated
    default:
        return QSortFilterProxyModel::data(index,role);
    }
}
