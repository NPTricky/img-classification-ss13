#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include "SamaelDockWidget.h"

class QTreeView;
class FileExplorerListView;
class FileExplorerTreeProxyModel;
class FileExplorerListProxyModel;
class SamaelImage;
class ThumbnailIconProvider;

class TreeWidget : public SamaelDockWidget
{
    Q_OBJECT

public:
    TreeWidget(QWidget *parent);
    ~TreeWidget();

private:
    void load(QDir directory);
    void load(QString path);
    void createActions();

    // QActions
    QAction *m_CollapseAction;

    // QWidgets
    QTreeView* m_TreeView;
    QVBoxLayout* m_Layout;
    QToolBar* m_ToolBar;
    FileExplorerListView* m_ListView;

    QFileSystemModel* m_FileSystemModel;
    ThumbnailIconProvider* m_IconProvider;
    FileExplorerTreeProxyModel* m_TreeProxyModel;
    FileExplorerListProxyModel* m_ListProxyModel;
    QRegExp m_FiltersRegExp;
    QStringList m_FiltersByName;

public slots:

    void openFiles();
    void openDirectory();

signals:

    void addImageToDatabase(std::string className, SamaelImage *image);
    void removeClassFromDatabase(std::string className);
};

#endif // TREEWIDGET_H
