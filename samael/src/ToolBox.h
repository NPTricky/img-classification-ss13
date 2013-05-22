#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "SamaelDockWidget.h"
class SamaelItemModel;
class QToolBox;

class ToolBox : public SamaelDockWidget
{
    Q_OBJECT

public:
    ToolBox(QWidget *parent);
    ~ToolBox();

private:
    void createActions();

    QToolBox *m_ToolBox;
	QVBoxLayout *m_Layout;

    // ToolBox Group "Compute" (GC = Group Compute)
    QWidget *m_ComputeContent;
    QGridLayout *m_ComputeContentLayout;
    QComboBox *m_GCKeyPointComboBox;
    QPushButton *m_GCKeyPointButton;
    QComboBox *m_GCDescriptorComboBox;
    QPushButton *m_GCDescriptorButton;

    // ToolBox Group "Analyze" (GA = Group Analyze)
    QWidget *m_AnalyzeContent;
    QVBoxLayout *m_AnalyzeContentLayout;

    // ToolBox Group "Test" (GT = Group Test)
    QWidget *m_TestContent;
    QVBoxLayout *m_TestContentLayout;
};

#endif // TOOLBOX_H
