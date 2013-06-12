#include "stdafx.h"
#include "FileExplorerListView.h"
#include "FileExplorerTreeProxyModel.h"

FileExplorerListView::FileExplorerListView(QWidget *parent, FileExplorerListProxyModel *list)
    : QListView(parent)
    , m_ListProxyModel(list)
{

}

FileExplorerListView::~FileExplorerListView()
{

}

void FileExplorerListView::setRootIndexProxy(const QModelIndex &index)
{
    QModelIndex index_list, index_source;

    const QAbstractProxyModel* model = dynamic_cast<const QAbstractProxyModel*>(index.model()); ///< [FileExplorerTreeProxyModel]

    if (model)
        index_source = model->mapToSource(index); ///< [QFileSystemModel]
    
    index_list = m_ListProxyModel->mapFromSource(index_source);

    setRootIndex(index_list);


    auto del = QAbstractItemView::model();
}
