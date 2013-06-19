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

  void classifierChanged(int i);

signals:

  void getClassNames(std::vector<std::string> &out_classNames);
  void getTrainingImages(std::map<std::string, std::vector<SamaelImage*>> &out_images);
  void getClassifyImages(std::map<std::string, std::vector<SamaelImage*>> &out_images);

  void visualizeKeypoints();
  void visualizeImage();
  void visualizeParallelCoordinates();

  void setFeatureDetector(int featureDetector);
  void getFeatureDetector(int &featureDetector); 
  void createVocabulary(std::map<std::string, std::vector<SamaelImage*>> &images);
  void trainClassifier(std::map<std::string, std::vector<SamaelImage*>> &images);
  void trainSVM();
  void classify(std::map<std::string, std::vector<SamaelImage*>> &images, std::vector<std::string> &classNames);
};

#endif // TOOLBOX_H
