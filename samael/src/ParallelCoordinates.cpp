#include "stdafx.h"
#include "ParallelCoordinates.h"

#include <limits.h>

#include "Shader.h"

#define COLORNORM 1.0f/255.0f

ParallelCoordinates::ParallelCoordinates(float scale) : m_scale(scale)
{
  glGenBuffers(1, &m_lineVBO);
  glGenBuffers(1, &m_diagramVBO);

  m_parallelCoordinatesVisualizer = new Shader("shader/parallelCoordinatesVisualizer.vert", "shader/parallelCoordinatesVisualizer.frag", "shader/parallelCoordinatesVisualizer.geom", nullptr, nullptr, nullptr);

  glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 2);
  glVertexAttribBinding(1, 1);
  glVertexAttribBinding(2, 1);
}

ParallelCoordinates::~ParallelCoordinates()
{
  glDeleteBuffers(1, &m_lineVBO);
  glDeleteBuffers(1, &m_diagramVBO);

  delete m_parallelCoordinatesVisualizer;
}

void ParallelCoordinates::setKeyPoints(cv::Mat keyPoints)
{
  m_featureNumber = keyPoints.rows;
  m_dimensionNumber = keyPoints.cols;

  float dataMin = std::numeric_limits<float>::max(), dataMax = std::numeric_limits<float>::min();

  for(unsigned int i = 0; i < m_featureNumber; i++)
  {
    for(unsigned int j = 0; j < m_dimensionNumber; j++)
    {
      dataMax = std::max(dataMax, ((float*)keyPoints.data)[i * m_dimensionNumber + j]);
      dataMin = std::min(dataMin, ((float*)keyPoints.data)[i * m_dimensionNumber + j]);
    }
  }

  float average = dataMax - dataMin;

  float *data = new float[m_featureNumber * m_dimensionNumber * 5];
  int index;
  float r,g,b;

  for(unsigned int i = 0; i < m_featureNumber; i++)
  {
    r = float(rand() % 256) * COLORNORM;
    g = float(rand() % 256) * COLORNORM;
    b = float(rand() % 256) * COLORNORM;
    for(unsigned int j = 0; j < m_dimensionNumber; j++)
    {
      index = 5 * i * m_dimensionNumber + 5 * j;
      data[index + 0] = (float(j) / float(m_dimensionNumber) * 2) * m_scale;
      data[index + 1] = (((float*)keyPoints.data)[i * m_dimensionNumber + j] - dataMin) / average;
      data[index + 2] = r;
      data[index + 3] = g;
      data[index + 4] = b;
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
  glBufferData(GL_ARRAY_BUFFER, m_featureNumber * m_dimensionNumber * 5 * sizeof(float), data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  delete[] data;

  data = new float[m_dimensionNumber * 10];

  //Dimension Axes
  for(unsigned int j = 0; j < m_dimensionNumber; j++)
  {
    data[10 * j + 0] = (float(j) / float(m_dimensionNumber) * 2) * m_scale;
    data[10 * j + 1] = 0.0f;

    data[10 * j + 2] = 0.0f;
    data[10 * j + 3] = 0.0f;
    data[10 * j + 4] = 0.0f;

    data[10 * j + 5] = (float(j) / float(m_dimensionNumber) * 2) * m_scale + 0.00001f;//trick to use the same shader as for the normal lines
    data[10 * j + 6] = 1.0f;

    data[10 * j + 7] = 0.0f;
    data[10 * j + 8] = 0.0f;
    data[10 * j + 9] = 0.0f;
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_diagramVBO);
  glBufferData(GL_ARRAY_BUFFER, m_dimensionNumber * 10 * sizeof(float), data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  delete[] data;
}

void ParallelCoordinates::visualize()
{
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  m_parallelCoordinatesVisualizer->useShader();
  glBindVertexBuffer(1, m_diagramVBO, 0, 5 * sizeof(float));
  glDrawArrays(GL_LINES, 0, m_dimensionNumber * 2);

  glBindVertexBuffer(1, m_lineVBO, 0, 5 * sizeof(float));
  glDrawArrays(GL_LINE_STRIP, 0, m_featureNumber * m_dimensionNumber);

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}