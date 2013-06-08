#ifndef PARALLELCOORDINATES_H_
#define PARALLELCOORDINATES_H_

#include <vector>

class Shader;

class ParallelCoordinates
{
public:

	ParallelCoordinates(float scale);
	~ParallelCoordinates();

  void setKeyPoints(cv::Mat keyPoints);

  void visualize();

private:

	ParallelCoordinates(const ParallelCoordinates&) {}
  ParallelCoordinates& operator=(const ParallelCoordinates&) {}

  GLuint m_lineVBO;
  GLuint m_diagramVBO;

  float m_scale;
  unsigned int m_dimensionNumber;
  unsigned int m_featureNumber;

  Shader *m_parallelCoordinatesVisualizer;
};
#endif