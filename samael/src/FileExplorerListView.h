#ifndef FILEEXPLORERLISTVIEW_H
#define FILEEXPLORERLISTVIEW_H

#include <QListView>
#include "FileExplorerListProxyModel.h"

class FileExplorerListView : public QListView
{
    Q_OBJECT

public:
    FileExplorerListView(QWidget *parent, FileExplorerListProxyModel *list);
    ~FileExplorerListView();

public slots:
    virtual void setRootIndexProxy(const QModelIndex &index);

private:
    FileExplorerListProxyModel *m_ListProxyModel;
};

#endif // FILEEXPLORERLISTVIEW_H
