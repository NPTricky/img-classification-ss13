#include "stdafx.h"
#include "VisualizerWidget.h"
#include "VisualizationManager.h"

#include "Shader.h"
#include "Texture.h"
#include "Renderquad.h"

VisualizationManager::VisualizationManager()
{
  m_painter = new QPainter();
}

VisualizationManager::~VisualizationManager()
{
  std::map<unsigned int, std::list<QImage*>>::iterator imageMapIterator = m_imageList.begin(), imageMapIteratorEnd = m_imageList.end();
  for(; imageMapIterator != imageMapIteratorEnd; imageMapIterator++)
  {
    std::list<QImage*>::iterator imageIterator = imageMapIterator->second.begin(), imageIteratorEnd = imageMapIterator->second.end();
    for(; imageIterator != imageIteratorEnd; imageIterator++)
    {
      delete *imageIterator;
    }
    imageMapIterator->second.clear();
  }
  m_imageList.clear();

  std::map<unsigned int, std::list<QPoint*>>::iterator keyPointMapIterator = m_keyPointList.begin(), keyPointMapIteratorEnd = m_keyPointList.end();
  for(; keyPointMapIterator != keyPointMapIteratorEnd; keyPointMapIterator++)
  {
    std::list<QPoint*>::iterator keyPointIterator = keyPointMapIterator->second.begin(), keyPointIteratorEnd = keyPointMapIterator->second.end();
    for(; keyPointIterator != keyPointIteratorEnd; keyPointIterator++)
    {
      delete *keyPointIterator;
    }
    keyPointMapIterator->second.clear();
  }
  m_keyPointList.clear();

  std::map<unsigned int, std::list<QRectF*>>::iterator bBoxMapIterator = m_bBoxList.begin(), bBoxMapIteratorEnd = m_bBoxList.end();
  for(; bBoxMapIterator != bBoxMapIteratorEnd; bBoxMapIterator++)
  {
    std::list<QRectF*>::iterator bBoxIterator = bBoxMapIterator->second.begin(), bBoxIteratorEnd = bBoxMapIterator->second.end();
    for(; bBoxIterator != bBoxIteratorEnd; bBoxIterator++)
    {
      delete *bBoxIterator;
    }
    bBoxMapIterator->second.clear();
  }
  m_bBoxList.clear();

  delete m_painter;
}

VisualizationManager* VisualizationManager::getInstance()
{
  static VisualizationManager instance;
  return &instance;
}

void VisualizationManager::connectWidget(VisualizerWidget *visualizerWidget)
{
  QObject::connect(visualizerWidget, SIGNAL(doPaint(QPaintDevice*)), this, SLOT(paint(QPaintDevice*)));
}

void VisualizationManager::disconnectWidget(VisualizerWidget *visualizerWidget)
{
  QObject::disconnect(visualizerWidget, SIGNAL(doPaint(QPaintDevice*)), this, SLOT(paint(QPaintDevice*)));
}

void VisualizationManager::addImage(QPaintDevice *device, QImage *image)
{
  unsigned int id = reinterpret_cast<unsigned int>(device);
  m_imageList[id].push_back(image);
}

void VisualizationManager::removeImage(QPaintDevice *device, QImage *image)
{
  unsigned int id = reinterpret_cast<unsigned int>(device);
  m_imageList[id].remove(image);
}

void VisualizationManager::addKeyPoint(QPaintDevice *device, QPoint *keyPoint)
{
  unsigned int id = reinterpret_cast<unsigned int>(device);
  m_keyPointList[id].push_back(keyPoint);
}

void VisualizationManager::removeKeyPoint(QPaintDevice *device, QPoint *keyPoint)
{
  unsigned int id = reinterpret_cast<unsigned int>(device);
  m_keyPointList[id].remove(keyPoint);
}

void VisualizationManager::addBoundingBox(QPaintDevice *device, QRectF *bBox)
{
  unsigned int id = reinterpret_cast<unsigned int>(device);
  m_bBoxList[id].push_back(bBox);
}

void VisualizationManager::removeBoundingBox(QPaintDevice *device, QRectF *bBox)
{
  unsigned int id = reinterpret_cast<unsigned int>(device);
  m_bBoxList[id].remove(bBox);
}

void VisualizationManager::paint(QPaintDevice *device)
{
  //unsigned int id = reinterpret_cast<unsigned int>(device);

  //m_painter->begin(device);

  //{
  //  m_painter->beginNativePainting();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //  m_painter->endNativePainting();
  //}

  //std::list<QImage*>::iterator imageIterator = m_imageList[id].begin(), imageIteratorEnd = m_imageList[id].end();
  //for(; imageIterator != imageIteratorEnd; imageIterator++)
  //{
  //  m_painter->drawImage(0, 0, **imageIterator);
  //}

  //std::list<QPoint*>::iterator keyPointIterator = m_keyPointList[id].begin(), keyPointIteratorEnd = m_keyPointList[id].end();
  //for(; keyPointIterator != keyPointIteratorEnd; keyPointIterator++)
  //{
  //  m_painter->drawPoint(**keyPointIterator);
  //}

  //std::list<QRectF*>::iterator bBoxIterator = m_bBoxList[id].begin(), bBoxIteratorEnd = m_bBoxList[id].end();
  //for(; bBoxIterator != bBoxIteratorEnd; bBoxIterator++)
  //{
  //  m_painter->drawRect(**bBoxIterator);
  //}
  //m_painter->end();
}