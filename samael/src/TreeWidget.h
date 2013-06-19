#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include "SamaelDockWidget.h"

class QTreeView;
class FileExplorerListView;
class FileExplorerTreeProxyModel;
class FileExplorerListProxyModel;
class SamaelImage;

class TreeWidget : public SamaelDockWidget
{
    Q_OBJECT

public:
    TreeWidget(QWidget *parent);
    ~TreeWidget();

private:
    void load(QDir directory);
    void load(QString file);
    void createActions();

    // QActions
    QAction *m_CollapseAction;

    // QWidgets
    QTreeView* m_TreeView;
    QVBoxLayout* m_Layout;
    QToolBar* m_ToolBar;
    FileExplorerListView* m_ListView;

    QFileSystemModel* m_FileSystemModel;
    FileExplorerTreeProxyModel* m_TreeProxyModel;
    FileExplorerListProxyModel* m_ListProxyModel;
    QFileInfo* m_Info;
    QStringList m_Filters;

public slots:

    void openFiles();
    void openDirectory();

signals:

    void saveImage(std::string className, SamaelImage *image);
    void removeImages(std::string className);
};

#endif // TREEWIDGET_H
