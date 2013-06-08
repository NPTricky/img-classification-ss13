#ifndef VISUALIZERWIDGET_H_
#define VISUALIZERWIDGET_H_

#include <QGLWidget>

#include <list>

class Renderquad;
class Shader;
class SamaelImage;
class ParallelCoordinates;

class VisualizerWidget : public QGLWidget
{
  Q_OBJECT

public:

  VisualizerWidget(QWidget *parent = nullptr);
  virtual ~VisualizerWidget() = 0;

  void addImage(SamaelImage *image);
  void addImages(std::vector<SamaelImage*> &images);
  void removeImage(SamaelImage *image);
  void removeAllImages();

  void addKeyPoint(cv::Point2f *keyPoint);
  void addKeyPoints(std::vector<cv::Point2f*> &keyPoints);
  void removeKeyPoint(cv::Point2f *keyPoint);
  void removeAllKeyPoints();

  void addBoundingBox(cv::Rect_<float> *bBox);
  void addBoundingBoxes(std::vector<cv::Rect_<float>*> &bBoxes);
  void removeBoundingBox(cv::Rect_<float> *bBox);
  void removeAllBoundingBoxes();

  void updateGL();

protected:

  void initializeGL();

  void paintGL();

  void resizeGL(int width, int height);

  void paintEvent(QPaintEvent *paintEvent);

  ParallelCoordinates *m_parallelCoordinates;

  Renderquad *m_renderQuad;

  Shader *m_imageVisualizer;
  Shader *m_keypointVisualizer;
  Shader *m_bBoxVisualizer;

  //container
  std::list<SamaelImage*> m_imageList;
  std::list<cv::Point2f*> m_keyPointList;
  std::list<cv::Rect_<float>*> m_bBoxList;

  static const unsigned int m_MAX_POINTS = 2048;

  GLuint m_keyPointVBO;
  GLuint m_keyPointNumber;

  GLuint m_bBoxVBO;
  GLuint m_bBoxNumber;
};

#endif