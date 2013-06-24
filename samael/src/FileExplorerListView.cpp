#include "stdafx.h"
#include "FileExplorerListView.h"
#include "FileExplorerTreeProxyModel.h"

FileExplorerListView::FileExplorerListView(QWidget *parent)
    : QListView(parent)
{

}

FileExplorerListView::~FileExplorerListView()
{

}

void FileExplorerListView::setRootIndexProxy(const QModelIndex &index)
{
    QModelIndex indexResult, indexSource;

    const QAbstractProxyModel* model = dynamic_cast<const QAbstractProxyModel*>(index.model()); ///< [FileExplorerTreeProxyModel]

    if (model)
        indexSource = model->mapToSource(index); ///< [QFileSystemModel]
    
    indexResult = m_ListProxyModel->mapFromSource(indexSource);

    setRootIndex(indexResult);
}

void FileExplorerListView::setModel(FileExplorerListProxyModel *model)
{
    m_ListProxyModel = model;
    QAbstractItemView::setModel(model);
}
