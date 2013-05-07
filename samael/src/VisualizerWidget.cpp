#include "stdafx.h"
#include "VisualizerWidget.h"

VisualizerWidget::VisualizerWidget(QWidget *parent) : QGLWidget(parent)
{
}

VisualizerWidget::~VisualizerWidget()
{
}

void VisualizerWidget::paintEvent(QPaintEvent *paintEvent)
{
  emit doPaint(this);
}