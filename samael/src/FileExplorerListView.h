#ifndef FILEEXPLORERLISTVIEW_H
#define FILEEXPLORERLISTVIEW_H

#include <QListView>
#include "FileExplorerListProxyModel.h"

class FileExplorerListView : public QListView
{
    Q_OBJECT

public:
    FileExplorerListView(QWidget *parent);
    ~FileExplorerListView();

    virtual void setModel(FileExplorerListProxyModel *model);

public slots:
    void setRootIndexProxy(const QModelIndex &index);

private:
    FileExplorerListProxyModel *m_ListProxyModel;

};

#endif // FILEEXPLORERLISTVIEW_H
