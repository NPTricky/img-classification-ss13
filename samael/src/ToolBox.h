#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "SamaelDockWidget.h"
class SamaelItemModel;
class QToolBox;
class SamaelImage;

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

    // ToolBox Group "Analyze" (GA = Group Analyze)
    QWidget *m_AnalyzeContent;
    QGridLayout *m_AnalyzeContentLayout;

    QPushButton *m_GCVisualizeKeypointsButton;
    QPushButton *m_GCVisualizeImageButton;
    QPushButton *m_GCVisualizeParallelCoordinatesButton;

    // ToolBox Group "Testing" (GT = Group Testing)
    QWidget *m_TestContent;
    QGridLayout *m_TestContentLayout;

    QComboBox *m_GCDescriptorComboBox;
    QPushButton *m_GCTestingButton;

private slots:

  void TestClassification();

  void classifierChanged(int i);

signals:
  void visualizeKeypoints();
  void visualizeImage();
  void visualizeParallelCoordinates();

  void setFeatureDetector(int featureDetector);
  void getFeatureDetector(int &featureDetector); 

  void doClassification();
};

#endif // TOOLBOX_H
