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
    connect(m_GCClassifyButton, SIGNAL(pressed()), this, SLOT(classifyBOW()));

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
    connect(m_GCDescriptorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(classifierChanged(int)));

    m_GCTrainingButton = new QPushButton(tr("Train"), m_TrainContent);
    connect(m_GCTrainingButton, SIGNAL(pressed()), this, SLOT(trainBOW()));

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
  std::vector<std::string> classNames;
  std::map<std::string, std::vector<SamaelImage*>> images;

  emit getClassNames(classNames);

  emit getTrainingImages(images);

  emit createVocabulary(images);
  emit trainClassifier(images);
  emit trainSVM();
}

void ToolBox::classifyBOW()
{
  std::vector<std::string> classNames;
  std::vector<std::string> classifiedClassNames;
  std::map<std::string, std::vector<SamaelImage*>> images;

  emit getClassNames(classNames);

  emit getClassifyImages(images);
  emit classify(images, classifiedClassNames);
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