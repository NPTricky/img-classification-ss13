#include "stdafx.h"
#include "VisualizerWidget.h"

#include <assert.h>

#include "SamaelImage.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderquad.h"

VisualizerWidget::VisualizerWidget(QWidget *parent) : QGLWidget(QGLFormat(QFlags<QGL::FormatOption>(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::StencilBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::NoDeprecatedFunctions)), parent)
{
  format().setVersion(4, 3);
  format().setPlane(0);
  //format().setDirectRendering(true);
  format().setStencil(false);
  format().setDepthBufferSize(32);
  //format().setStencilBufferSize(stencilBufferSize);
  format().setRedBufferSize(8);
  format().setGreenBufferSize(8);
  format().setBlueBufferSize(8);
  format().setAlphaBufferSize(8);
}

VisualizerWidget::~VisualizerWidget()
{
  removeAllImages();

  removeAllKeyPoints();

  removeAllBoundingBoxes();

  delete m_imageVisualizer;
  delete m_keypointVisualizer;
  delete m_bBoxVisualizer;
  delete m_renderQuad;
}
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
void VisualizerWidget::addImage(SamaelImage *image)
{
  m_imageList.push_back(image);
}

void VisualizerWidget::addImages(std::vector<SamaelImage*> &images)
{
  for(int i = 0; i < images.size(); i++)
  {
    m_imageList.push_back(images[i]);
  }
}

void VisualizerWidget::removeImage(SamaelImage *image)
{
  m_imageList.remove(image);
}

void VisualizerWidget::removeAllImages()
{
  std::list<SamaelImage*>::iterator imageIterator = m_imageList.begin(), imageIteratorEnd = m_imageList.end();
  for(; imageIterator != imageIteratorEnd; imageIterator++)
  {
    delete *imageIterator;
  }
  m_imageList.clear();
}
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
void VisualizerWidget::addKeyPoint(cv::Point2f *keyPoint)
{
  m_keyPointList.push_back(keyPoint);

  glBindBuffer(GL_ARRAY_BUFFER, m_keyPointVBO);
  glBufferSubData(GL_ARRAY_BUFFER, m_keyPointNumber * sizeof(cv::Point2f), sizeof(cv::Point2f), &keyPoint->x);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  m_keyPointNumber++;
}

void VisualizerWidget::addKeyPoints(std::vector<cv::Point2f*> &keyPoints)
{
  for(int i = 0; i < keyPoints.size(); i++)
  {
    m_keyPointList.push_back(keyPoints[i]);
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_keyPointVBO);
  glBufferSubData(GL_ARRAY_BUFFER, m_keyPointNumber * sizeof(cv::Point2f), keyPoints.size() * sizeof(cv::Point2f), &keyPoints[0]->x);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  m_keyPointNumber += keyPoints.size();
}

void VisualizerWidget::removeKeyPoint(cv::Point2f *keyPoint)
{
  m_keyPointList.remove(keyPoint);

  std::list<cv::Point2f*>::iterator keyPointIterator = m_keyPointList.begin(), keyPointIteratorEnd = m_keyPointList.end();
  for(; keyPointIterator != keyPointIteratorEnd; keyPointIterator++)
  {
    addKeyPoint(*keyPointIterator);
  }

  m_keyPointNumber--;
}

void VisualizerWidget::removeAllKeyPoints()
{
  std::list<cv::Point2f*>::iterator keyPointIterator = m_keyPointList.begin(), keyPointIteratorEnd = m_keyPointList.end();
  for(; keyPointIterator != keyPointIteratorEnd; keyPointIterator++)
  {
    delete *keyPointIterator;
  }
  m_keyPointList.clear();

  glDeleteBuffers(1, &m_keyPointVBO);
  m_keyPointNumber = 0;
}
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
void VisualizerWidget::addBoundingBox(cv::Rect_<float> *bBox)
{
  m_bBoxList.push_back(bBox);

  glBindBuffer(GL_ARRAY_BUFFER, m_bBoxVBO);
  glBufferSubData(GL_ARRAY_BUFFER, m_bBoxNumber * sizeof(cv::Rect_<float>), sizeof(cv::Rect_<float>), &bBox->x);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  m_bBoxNumber++;
}

void VisualizerWidget::addBoundingBoxes(std::vector<cv::Rect_<float>*> &bBoxes)
{
  for(int i = 0; i < bBoxes.size(); i++)
  {
    addBoundingBox(bBoxes[i]);
  }
}

void VisualizerWidget::removeBoundingBox(cv::Rect_<float> *bBox)
{
  m_bBoxList.remove(bBox);

  std::list<cv::Rect_<float>*>::iterator bBoxIterator = m_bBoxList.begin(), bBoxIteratorEnd = m_bBoxList.end();
  for(; bBoxIterator != bBoxIteratorEnd; bBoxIterator++)
  {
    addBoundingBox(*bBoxIterator);
  }

  m_bBoxNumber--;
}

void VisualizerWidget::removeAllBoundingBoxes()
{
  std::list<cv::Rect_<float>*>::iterator bBoxIterator = m_bBoxList.begin(), bBoxIteratorEnd = m_bBoxList.end();
  for(; bBoxIterator != bBoxIteratorEnd; bBoxIterator++)
  {
    delete *bBoxIterator;
  }
  m_bBoxList.clear();

  glDeleteBuffers(1, &m_bBoxVBO);
  m_bBoxNumber = 0;
}
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
void VisualizerWidget::initializeGL()
{
  GLenum err = glewInit();
  char errString[1024];
  if(err != GLEW_OK)
  {
    sprintf(errString, "Error: %s\n", glewGetErrorString(err));
  }

  m_imageVisualizer = new Shader("shader/imageVisualizer.vert", "shader/imageVisualizer.frag", nullptr, nullptr, nullptr, nullptr);
  m_keypointVisualizer = new Shader("shader/keypointVisualizer.vert", "shader/keypointVisualizer.frag", "shader/keypointVisualizer.geom", nullptr, nullptr, nullptr);
  m_bBoxVisualizer = new Shader("shader/bBoxVisualizer.vert", "shader/bBoxVisualizer.frag", nullptr, nullptr, nullptr, nullptr);

  m_renderQuad = new Renderquad;

  glGenBuffers(1, &m_keyPointVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_keyPointVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cv::Point2f) * m_MAX_POINTS, nullptr, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  m_keyPointNumber = 0;

  glGenBuffers(1, &m_bBoxVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_keyPointVBO);
  glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(cv::Point2f) * m_MAX_POINTS, nullptr, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  m_bBoxNumber = 0;

  glEnable(GL_DEPTH_TEST);
  glClearDepth(0.0f);
  glDepthFunc(GL_GREATER);
  glClearColor(0.73f, 0.91f, 0.0f, 1.0f);
}
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
void VisualizerWidget::updateGL()
{
}
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
void VisualizerWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_imageVisualizer->useShader();
  m_imageVisualizer->setTexture(0, 0);

  std::list<SamaelImage*>::iterator imageIterator = m_imageList.begin(), imageIteratorEnd = m_imageList.end();
  for(; imageIterator != imageIteratorEnd; imageIterator++)
  {
    (*imageIterator)->getTexture().setTexture(0);
    m_renderQuad->render(true);
  }

  m_keypointVisualizer->useShader();

  std::list<cv::Point2f*>::iterator keyPointIterator = m_keyPointList.begin(), keyPointIteratorEnd = m_keyPointList.end();
  for(; keyPointIterator != keyPointIteratorEnd; keyPointIterator++)
  {
  }

  m_bBoxVisualizer->useShader();

  std::list<cv::Rect_<float>*>::iterator bBoxIterator = m_bBoxList.begin(), bBoxIteratorEnd = m_bBoxList.end();
  for(; bBoxIterator != bBoxIteratorEnd; bBoxIterator++)
  {
  }
}
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
void VisualizerWidget::resizeGL(int width, int height)
{
  resize(width, height);
  int side = qMin(width, height);
  glViewport(0, 0, width, height);
}
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
void VisualizerWidget::paintEvent(QPaintEvent *paintEvent)
{
  glDraw();
}