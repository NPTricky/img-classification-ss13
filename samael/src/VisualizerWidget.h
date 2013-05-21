#ifndef VISUALIZERWIDGET_H_
#define VISUALIZERWIDGET_H_

#include <QGLWidget>

class VisualizerWidget : public QGLWidget
{
  Q_OBJECT

public:

  VisualizerWidget(QWidget *parent = nullptr);
  virtual ~VisualizerWidget() = 0;

  void updateGL();

protected:

  void initializeGL();

  void paintGL();

  void resizeGL(int width, int height);

  void paintEvent(QPaintEvent *paintEvent);

signals:

  void doPaint(QPaintDevice *device);
};

#endif