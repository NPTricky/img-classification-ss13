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
    void createActions();

    // QActions
    QAction *m_ExpandAction;
    QAction *m_CollapseAction;

    // QWidgets
    QTreeView* m_TreeView;
    QVBoxLayout* m_Layout;
    QToolBar* m_ToolBar;

    SamaelItemModel* m_SamaelItemModel;

public slots:
    void open();
};

#endif // TREEWIDGET_H
