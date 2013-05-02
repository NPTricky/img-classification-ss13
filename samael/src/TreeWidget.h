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
    SamaelItemModel* m_SamaelItemModel;
    QTreeView* m_TreeView;
    QVBoxLayout* m_Layout;

public slots:
    void open();
};

#endif // TREEWIDGET_H
