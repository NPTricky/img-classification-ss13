#ifndef FILEEXPLORERLISTVIEW_H
#define FILEEXPLORERLISTVIEW_H

#include <QListView>
class FileExplorerListProxyModel;

class FileExplorerListView : public QListView
{
    Q_OBJECT

public:
    FileExplorerListView(QWidget *parent);
    ~FileExplorerListView();

    void setModel(FileExplorerListProxyModel *model);

public slots:
    void setRootIndexProxy(const QModelIndex &index);
    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    FileExplorerListProxyModel *m_ListProxyModel;

};

#endif // FILEEXPLORERLISTVIEW_H
