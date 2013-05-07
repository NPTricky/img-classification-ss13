#include "stdafx.h"
#include "VisualizerWidget.h"

VisualizerWidget::VisualizerWidget(QWidget *parent) : QGLWidget(parent)
{
}

VisualizerWidget::~VisualizerWidget()
{
}

void VisualizerWidget::initializeGL()
{
}

void VisualizerWidget::updateGL()
{
}

void VisualizerWidget::paintGL()
{
}

void VisualizerWidget::paintEvent(QPaintEvent *paintEvent)
{
  emit doPaint(this);
}