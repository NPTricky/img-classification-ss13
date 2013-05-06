#ifndef VISUALIZERWIDGET_H_
#define VISUALIZERWIDGET_H_

#include <QWidget>

class VisualizerWidget : public QWidget
{
  Q_OBJECT

public:

  VisualizerWidget(QWidget *parent = nullptr);
  virtual ~VisualizerWidget() = 0;

  void paintEvent(QPaintEvent *paintEvent);

signals:

    void doPaint(QPaintDevice *device);
};

#endif