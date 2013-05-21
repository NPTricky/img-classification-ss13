#include "stdafx.h"
#include "VisualizerWidget.h"

#include <assert.h>

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
}

void VisualizerWidget::initializeGL()
{
  assert(glewInit() == GLEW_OK);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(0.0f);
  glDepthFunc(GL_GREATER);
  glClearColor(0.73f, 0.91f, 0.0f, 1.0f);
}

void VisualizerWidget::updateGL()
{
}

void VisualizerWidget::paintGL()
{
  emit doPaint(this);
}

void VisualizerWidget::resizeGL(int width, int height)
{
  resize(width, height);
  int side = qMin(width, height);
  glViewport(0, 0, width, height);
}

void VisualizerWidget::paintEvent(QPaintEvent *paintEvent)
{
  glDraw();
  //emit doPaint(this);
}