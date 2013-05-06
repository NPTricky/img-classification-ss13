#include "stdafx.h"
#include "VisualizerWidget.h"

VisualizerWidget::VisualizerWidget(QWidget *parent) : QWidget(parent, Qt::Widget)
{
}

VisualizerWidget::~VisualizerWidget()
{
}

void VisualizerWidget::paintEvent(QPaintEvent *paintEvent)
{
  emit doPaint(this);
}