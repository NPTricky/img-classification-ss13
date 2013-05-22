#include "stdafx.h"
#include "ToolBox.h"
#include "SamaelItemModel.h"
#include "Logger.h"
#include "SamaelImage.h"

ToolBox::ToolBox(QWidget *parent)
    : SamaelDockWidget(parent, QStringLiteral("ToolBox"), QStringLiteral("Operations"))
{
    // configure the toolbox
    m_ToolBox = new QToolBox(m_ContentWidget);
    m_ToolBox->setMinimumWidth(200);

    // configure the toolbox group "compute" (GC = Group Compute)
    m_ComputeContent = new QWidget(m_ContentWidget);
    m_ComputeContentLayout = new QGridLayout(m_ComputeContent);
    m_ComputeContentLayout->setContentsMargins(0,0,0,0);
    m_GCKeyPointComboBox = new QComboBox(m_ComputeContent);
    m_GCKeyPointComboBox->addItem(tr("SIFT"));
    m_GCKeyPointComboBox->addItem(tr("SURF"));
    m_GCKeyPointComboBox->addItem(tr("MSER"));
    m_GCKeyPointButton = new QPushButton(tr("Find KeyPoints"), m_ComputeContent);
    m_GCDescriptorComboBox = new QComboBox(m_GCKeyPointComboBox);
    m_GCDescriptorComboBox->addItem(tr("SIFT"));
    m_GCDescriptorComboBox->addItem(tr("SURF"));
    m_GCDescriptorComboBox->addItem(tr("MSER"));
    m_GCDescriptorButton = new QPushButton(tr("Extract Descriptors"), m_ComputeContent);
    // ... ADD MORE STUFF
    m_ComputeContentLayout->addWidget(m_GCKeyPointComboBox,0,0);
    m_ComputeContentLayout->addWidget(m_GCKeyPointButton,0,1);
    m_ComputeContentLayout->addWidget(m_GCDescriptorComboBox,1,0);
    m_ComputeContentLayout->addWidget(m_GCDescriptorButton,1,1);
    //m_ComputeContentLayout->addWidget(,m_ComputeContentLayout->rowCount(),0);
    m_ComputeContent->setLayout(m_ComputeContentLayout);

    // configure the toolbox group "analyze" (GA = Group Analyze)
    m_AnalyzeContent = new QWidget(m_ContentWidget);
    m_AnalyzeContentLayout = new QVBoxLayout(m_AnalyzeContent);
    m_AnalyzeContentLayout->setContentsMargins(0,0,0,0);
    // ... ADD MORE STUFF
    m_ComputeContent->setLayout(m_AnalyzeContentLayout);

    // configure the toolbox group "test" (GT = Group Test)
    m_TestContent = new QWidget(m_ContentWidget);
    m_TestContentLayout = new QVBoxLayout(m_TestContent);
    m_TestContentLayout->setContentsMargins(0,0,0,0);
    // ... ADD MORE STUFF
    m_ComputeContent->setLayout(m_TestContentLayout);

    // add the toolbox groups to the toolbox
	m_ToolBox->addItem(m_ComputeContent, tr("Compute"));
	m_ToolBox->addItem(m_AnalyzeContent, tr("Analyze"));
    m_ToolBox->addItem(m_TestContent, tr("Test"));

	m_Layout = new QVBoxLayout(m_ContentWidget);
    m_Layout->setContentsMargins(0,0,0,0);
    m_Layout->addWidget(m_ToolBox);
    m_Layout->addStretch(1);
    finalise(m_Layout);

    QLOG_INFO() << "Toolbox - Ready!";
}

ToolBox::~ToolBox()
{

}

void ToolBox::createActions()
{
   
}
