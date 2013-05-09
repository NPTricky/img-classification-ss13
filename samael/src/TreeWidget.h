#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include "SamaelDockWidget.h"
class SamaelItemModel;
class QTreeView;

class TreeWidget : public SamaelDockWidget
{
    Q_OBJECT

public:
    TreeWidget(QWidget *parent);
    ~TreeWidget();

private:
    /// usability
    QModelIndex insertNodeReturnIndex(QString name, const QModelIndex &parent = QModelIndex());

    void load(QDir directory);
    void load(QString file, const QModelIndex &parent = QModelIndex());
    void createActions();

    // QActions
    QAction *m_ExpandAction;
    QAction *m_CollapseAction;

    // QWidgets
    QTreeView* m_TreeView;
    QVBoxLayout* m_Layout;
    QToolBar* m_ToolBar;
    QListView* m_ListView;

    QFileSystemModel* m_FileSystemModel;
    QFileInfo* m_Info;
    QStringList m_Filters;

public slots:
    void openFiles();
    void openDirectory();
};

#endif // TREEWIDGET_H
