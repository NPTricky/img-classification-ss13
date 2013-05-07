#ifndef VISUALIZERWIDGET_H_
#define VISUALIZERWIDGET_H_

#include <QGLWidget>

class VisualizerWidget : public QGLWidget
{
  Q_OBJECT

public:

  VisualizerWidget(QWidget *parent = nullptr);
  virtual ~VisualizerWidget() = 0;

  void initializeGL();

  void updateGL();

  void paintGL();

  void paintEvent(QPaintEvent *paintEvent);

signals:

    void doPaint(QPaintDevice *device);
};

#endif