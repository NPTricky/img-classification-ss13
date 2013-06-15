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

    // ToolBox Group "Compute" (GC = Group Compute)
    QWidget *m_ComputeContent;
    QGridLayout *m_ComputeContentLayout;

    QPushButton *m_GCClassifyButton;

    // ToolBox Group "Analyze" (GA = Group Analyze)
    QWidget *m_AnalyzeContent;
    QGridLayout *m_AnalyzeContentLayout;

    QPushButton *m_GCVisualizeKeypointsButton;
    QPushButton *m_GCVisualizeImageButton;
    QPushButton *m_GCVisualizeParallelCoordinatesButton;

    // ToolBox Group "Training" (GT = Group Training)
    QWidget *m_TrainContent;
    QGridLayout *m_TrainContentLayout;

    QComboBox *m_GCDescriptorComboBox;
    QPushButton *m_GCTrainingButton;

private slots:

  void trainBOW();
  void classifyBOW();

signals:

  void visualizeKeypoints();
  void visualizeImage();
  void visualizeParallelCoordinates();

  void setFeatureDetector(int featureDetector);
  void getFeatureDetector(int &featureDetector); 
  void trainClassifier(QString className, std::vector<SamaelImage*> &images);
  void classify(std::vector<SamaelImage*> &images, std::vector<QString> &classNames);
};

#endif // TOOLBOX_H
