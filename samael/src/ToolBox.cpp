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

    //m_ComputeContentLayout->addWidget(,m_ComputeContentLayout->rowCount(),0);

    // configure the toolbox group "analyze" (GA = Group Analyze)
    m_AnalyzeContent = new QWidget(m_ContentWidget);

    m_GCVisualizeKeypointsButton = new QPushButton(tr("Visualize Keypoints"), m_AnalyzeContent);
    m_GCVisualizeImageButton = new QPushButton(tr("Visualize Image"), m_AnalyzeContent);;
    m_GCVisualizeParallelCoordinatesButton = new QPushButton(tr("Visualize Parallel Coordinates"), m_AnalyzeContent);;

    m_AnalyzeContentLayout = new QGridLayout(m_AnalyzeContent);
    m_AnalyzeContentLayout->setContentsMargins(0,0,0,0);
    m_AnalyzeContentLayout->addWidget(m_GCVisualizeKeypointsButton,0,0);
    m_AnalyzeContentLayout->addWidget(m_GCVisualizeImageButton,1,0);
    m_AnalyzeContentLayout->addWidget(m_GCVisualizeParallelCoordinatesButton,2,0);

    // configure the toolbox group "training" (GT = Group Test)
    m_TestContent = new QWidget(m_ContentWidget);
    
    m_GCDescriptorComboBox = new QComboBox(m_TestContent);
    m_GCDescriptorComboBox->addItem(tr("SIFT"));
    m_GCDescriptorComboBox->addItem(tr("SURF"));
    m_GCDescriptorComboBox->addItem(tr("MSER"));
    connect(m_GCDescriptorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(classifierChanged(int)));

    m_GCTestingButton = new QPushButton(tr("Test"), m_TestContent);
    connect(m_GCTestingButton, SIGNAL(pressed()), this, SLOT(TestClassification()));

    m_TestContentLayout = new QGridLayout(m_TestContent);
    m_TestContentLayout->setContentsMargins(0,0,0,0);
    m_TestContentLayout->addWidget(m_GCDescriptorComboBox,1,0);
    m_TestContentLayout->addWidget(m_GCTestingButton,1,1);

    // add the toolbox groups to the toolbox
    m_ToolBox->addItem(m_TestContent, tr("Testing"));
    m_ToolBox->addItem(m_AnalyzeContent, tr("Analyze"));

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

void ToolBox::TestClassification()
{
  emit doClassification();
}

void ToolBox::classifierChanged(int i)
{
  switch(m_GCDescriptorComboBox->currentIndex())
  {
  case 0:
    emit setFeatureDetector(0);//SIFT
    break;
  case 1:
    emit setFeatureDetector(1);//SURF
    break;
  case 2:
    emit setFeatureDetector(2);//MSER
    break;
  }
}