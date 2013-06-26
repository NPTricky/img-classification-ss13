#include "stdafx.h"
#include "VisualizerWidget.h"

#include <assert.h>

#include "SamaelImage.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderquad.h"
#include "ParallelCoordinates.h"
#include "Logger.h"

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

  QLOG_INFO() << "VisualizerWidget - Ready!";
}

VisualizerWidget::~VisualizerWidget()
{
  removeAllImages();

  removeAllKeyPoints();

  removeAllBoundingBoxes();

  glDeleteBuffers(1, &m_keyPointVBO);
  glDeleteBuffers(1, &m_bBoxVBO);

  delete m_parallelCoordinates;

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

  m_keyPointNumber = 0;
}
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
void VisualizerWidget::addBoundingBox(cv::Rect_<float> *bBox)
{
  m_bBoxList.push_back(bBox);

  glBindBuffer(GL_ARRAY_BUFFER, m_bBoxVBO);
  cv::Point2f tmpPos = cv::Point2f(bBox->x, bBox->y);
  glBufferSubData(GL_ARRAY_BUFFER, m_bBoxNumber * sizeof(cv::Rect_<float>), sizeof(cv::Point2f), &tmpPos);
  tmpPos = cv::Point2f(bBox->x + bBox->width, bBox->y + bBox->height);
  glBufferSubData(GL_ARRAY_BUFFER, m_bBoxNumber * sizeof(cv::Rect_<float>) + sizeof(cv::Point2f), sizeof(cv::Point2f), &tmpPos);
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

  m_bBoxNumber = 0;
}
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
void VisualizerWidget::initializeGL()
{
  GLenum err = glewInit();
  
  if(err != GLEW_OK)
      QLOG_ERROR_NOCONTEXT() << "GLEW:" << glewGetErrorString(err);

  m_parallelCoordinates = new ParallelCoordinates(1.0f);

  int featureNumber = 3;
  int dimensionNumber = 128;
  float *data = new float[featureNumber * dimensionNumber];//{0.2f,0.3f,0.4f,0.6f, 0.1f,0.2f,0.3f,0.4f, 0.5f,0.6f,0.7f,0.8f};

  for(int i = 0; i < featureNumber; i++)
  {
    for(int j = 0; j < dimensionNumber; j++)
    {
      data[i * dimensionNumber + j] = rand() % 65536;
    }
  }

  m_parallelCoordinates->setKeyPoints(cv::Mat(featureNumber, dimensionNumber, CV_32F, data));

  m_imageVisualizer = new Shader("shader/imageVisualizer.vert", "shader/imageVisualizer.frag", nullptr, nullptr, nullptr, nullptr);
  m_keypointVisualizer = new Shader("shader/keypointVisualizer.vert", "shader/keypointVisualizer.frag", "shader/keypointVisualizer.geom", nullptr, nullptr, nullptr);
  m_bBoxVisualizer = new Shader("shader/bBoxVisualizer.vert", "shader/bBoxVisualizer.frag", "shader/bBoxVisualizer.geom", nullptr, nullptr, nullptr);

  glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(0, 0);

  m_renderQuad = new Renderquad;

  glGenBuffers(1, &m_keyPointVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_keyPointVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cv::Point2f) * m_MAX_POINTS, nullptr, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  m_keyPointNumber = 0;

  glGenBuffers(1, &m_bBoxVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_bBoxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cv::Rect_<float>) * m_MAX_POINTS, nullptr, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  m_bBoxNumber = 0;

  glEnable(GL_DEPTH_TEST);
  glClearDepth(0.0f);
  glDepthFunc(GL_GREATER);
  glClearColor(0.73f, 0.91f, 0.0f, 1.0f);

  cv::Point2f *testPoint = new cv::Point2f(0.5, 0.5);
  addKeyPoint(testPoint);

  cv::Rect_<float> *testBox = new cv::Rect_<float>(-0.25f, 0.25f, 0.5f, -0.5f);
  addBoundingBox(testBox);
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

  m_parallelCoordinates->visualize();

  m_imageVisualizer->useShader();

  std::list<SamaelImage*>::iterator imageIterator = m_imageList.begin(), imageIteratorEnd = m_imageList.end();
  for(; imageIterator != imageIteratorEnd; imageIterator++)
  {
    (*imageIterator)->getTexture().setTexture(0);
    m_imageVisualizer->setTexture(0, 0);
    m_renderQuad->render(true);
  }

  glEnableVertexAttribArray(0);

  m_keypointVisualizer->useShader();
  glBindVertexBuffer(0, m_keyPointVBO, 0, sizeof(cv::Point2f));
  glDrawArrays(GL_POINTS, 0, m_keyPointNumber);

  m_bBoxVisualizer->useShader();
  glBindVertexBuffer(0, m_bBoxVBO, 0, sizeof(cv::Point2f));
  glDrawArrays(GL_LINE_STRIP, 0, 2 * m_bBoxNumber);

  glDisableVertexAttribArray(0);

  const GLenum ErrorValue = glGetError();
	if(ErrorValue != GL_NO_ERROR) 
  {
		assert(false);//stop immediately, openGL error happened
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