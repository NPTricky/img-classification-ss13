#include "stdafx.h"
#include "FileExplorerListView.h"
#include "FileExplorerListProxyModel.h"

static const float ICON_SIZE_FACTOR = 1.5f;

FileExplorerListView::FileExplorerListView(QWidget *parent)
    : QListView(parent)
{
    auto size = viewport()->width()*ICON_SIZE_FACTOR;

    setSelectionMode(QAbstractItemView::SingleSelection);
    setViewMode(QListView::IconMode);
    setIconSize(QSize(size,size));
    //setSpacing(0);
    //setUniformItemSizes(true);
    setResizeMode(QListView::Adjust);
    setLayoutMode(QListView::Batched);
    setTextElideMode(Qt::ElideRight);
    setMovement(QListView::Static);
}

FileExplorerListView::~FileExplorerListView()
{

}

void FileExplorerListView::setRootIndexProxy(const QModelIndex &index)
{
    const QAbstractProxyModel* model = dynamic_cast<const QAbstractProxyModel*>(index.model()); ///< [FileExplorerTreeProxyModel]

    if (model)
    {
        QModelIndex indexSource = model->mapToSource(index); ///< [QFileSystemModel]
        QModelIndex indexResult = m_ListProxyModel->mapFromSource(indexSource);
        setRootIndex(indexResult);
    }
}

void FileExplorerListView::setModel(FileExplorerListProxyModel *model)
{
    m_ListProxyModel = model;
    QAbstractItemView::setModel(model);
}

void FileExplorerListView::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    setRootIndexProxy(current);
}
