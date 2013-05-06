#ifndef VISUALIZATIONMANAGER_H_
#define VISUALIZATIONMANAGER_H_

#include <QWidget>
#include <QObject>

#include <list>
#include <map>

class VisualizerWidget;

class VisualizationManager : public QObject
{
public:

  static VisualizationManager* getInstance();

  void connectWidget(VisualizerWidget *visualizerWidget);
  void disconnectWidget(VisualizerWidget *visualizerWidget);

  void addImage(QPaintDevice *device, QImage *image);
  void removeImage(QPaintDevice *device, QImage *image);

  void addKeyPoint(QPaintDevice *device, QPoint *keyPoint);
  void removeKeyPoint(QPaintDevice *device, QPoint *keyPoint);

  void addBoundingBox(QPaintDevice *device, QRectF *bBox);
  void removeBoundingBox(QPaintDevice *device, QRectF *bBox);

  Q_OBJECT
    
private:
  
  VisualizationManager();
  ~VisualizationManager();

  QPainter *m_painter;

  //container
  std::map<unsigned int, std::list<QImage*>> m_imageList;
  std::map<unsigned int, std::list<QPoint*>> m_keyPointList;
  std::map<unsigned int, std::list<QRectF*>> m_bBoxList;

public slots:

  void paint(QPaintDevice *device);
};

#endif