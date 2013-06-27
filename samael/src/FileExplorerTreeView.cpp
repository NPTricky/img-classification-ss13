#include "stdafx.h"
#include "FileExplorerTreeView.h"

FileExplorerTreeView::FileExplorerTreeView(QWidget *parent)
    : QTreeView(parent)
{
    //setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setIndentation(20);
}

FileExplorerTreeView::~FileExplorerTreeView()
{

}
