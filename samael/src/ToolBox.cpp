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

    m_GCClassifyButton = new QPushButton(tr("Classify"), m_ComputeContent);
    connect(m_GCClassifyButton, SIGNAL(triggered()), this, SLOT(classifyBOW()));

    m_ComputeContentLayout = new QGridLayout(m_ComputeContent);
    m_ComputeContentLayout->setContentsMargins(0,0,0,0);
    m_ComputeContentLayout->addWidget(m_GCClassifyButton,1,1);
    m_ComputeContent->setLayout(m_ComputeContentLayout);

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
    m_ComputeContent->setLayout(m_AnalyzeContentLayout);

    // configure the toolbox group "training" (GT = Group Test)
    m_TrainContent = new QWidget(m_ContentWidget);
    
    m_GCDescriptorComboBox = new QComboBox(m_TrainContent);
    m_GCDescriptorComboBox->addItem(tr("SIFT"));
    m_GCDescriptorComboBox->addItem(tr("SURF"));
    m_GCDescriptorComboBox->addItem(tr("MSER"));

    m_GCTrainingButton = new QPushButton(tr("Train"), m_TrainContent);
    connect(m_GCTrainingButton, SIGNAL(triggered()), this, SLOT(trainBOW()));

    m_TrainContentLayout = new QGridLayout(m_TrainContent);
    m_TrainContentLayout->setContentsMargins(0,0,0,0);
    m_TrainContentLayout->addWidget(m_GCDescriptorComboBox,1,0);
    m_TrainContentLayout->addWidget(m_GCTrainingButton,1,1);
    m_ComputeContent->setLayout(m_TrainContentLayout);

    // add the toolbox groups to the toolbox
    m_ToolBox->addItem(m_TrainContent, tr("Training"));
	  m_ToolBox->addItem(m_ComputeContent, tr("Compute"));
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

void ToolBox::trainBOW()
{
  switch(m_GCDescriptorComboBox->currentIndex())
  {
  case 0:
    emit setFeatureDetector(0);
    break;
  case 1:
    emit setFeatureDetector(1);
    break;
  case 2:
    emit setFeatureDetector(2);
    break;
  }

  //emit trainClassifier();
}

void ToolBox::classifyBOW()
{
  switch(m_GCDescriptorComboBox->currentIndex())
  {
  case 0:
    emit setFeatureDetector(0);
    break;
  case 1:
    emit setFeatureDetector(1);
    break;
  case 2:
    emit setFeatureDetector(2);
    break;
  }

  //emit classify();
}