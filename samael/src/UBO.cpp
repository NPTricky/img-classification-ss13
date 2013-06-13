#include "stdafx.h"
#include "UBO.h"
#include "Shader.h"

UBO::UBO() : m_uniformIndices(NULL),
	           m_uniformOffsets(NULL),
	           m_arrayStrides(NULL),
	           m_matrixStrides(NULL),
	           m_bufferData(NULL)
{
  
}

UBO::UBO(Shader *shader, const GLuint bufferSize, const GLuint uniformCount, GLenum usage, const char **uniformNames) : m_bufferDataOffset(0),//shared layout constructor
                                                                                                                               m_bufferBindingPoint(0),
                                                                                                                               m_uniformNames(uniformNames),
                                                                                                                               m_bufferSize(bufferSize),
                                                                                                                               m_uniformCount(uniformCount)
{
	glGenBuffers( 1, &m_bufferIndex );
	glBindBuffer( GL_UNIFORM_BUFFER, m_bufferIndex );
	glBufferData( GL_UNIFORM_BUFFER, this->m_bufferSize, NULL, usage );

  m_uniformIndices  = new GLuint[this->m_uniformCount];
	m_uniformOffsets  = new GLint[this->m_uniformCount];
	m_arrayStrides    = new GLint[this->m_uniformCount];
	m_matrixStrides   = new GLint[this->m_uniformCount];
	m_bufferData      = new unsigned char[bufferSize];

	glGetUniformIndices  ( shader->program, this->m_uniformCount, this->m_uniformNames, m_uniformIndices);
	glGetActiveUniformsiv( shader->program, this->m_uniformCount, m_uniformIndices, GL_UNIFORM_OFFSET,       m_uniformOffsets);
	glGetActiveUniformsiv( shader->program, this->m_uniformCount, m_uniformIndices, GL_UNIFORM_ARRAY_STRIDE, m_arrayStrides);
	glGetActiveUniformsiv( shader->program, this->m_uniformCount, m_uniformIndices, GL_UNIFORM_MATRIX_STRIDE,m_matrixStrides);//maybe useless?!
}

UBO::UBO(const GLuint bufferSize, const GLuint uniformCount, GLenum usage) : m_bufferDataOffset(0),//std140 layout constructor
                                                                             m_bufferSize(bufferSize),
                                                                             m_uniformCount(uniformCount),
                                                                             m_uniformIndices(NULL),
                                                                             m_arrayStrides(NULL),
                                                                             m_matrixStrides(NULL)
{
  glGenBuffers( 1, &m_bufferIndex );
	glBindBuffer( GL_UNIFORM_BUFFER, m_bufferIndex );
	glBufferData( GL_UNIFORM_BUFFER, this->m_bufferSize, NULL, usage );

  m_bufferData      = new unsigned char[this->m_bufferSize];
  m_uniformOffsets  = new GLint[1];
  m_uniformOffsets[0] = 0;//data is already aligned for std layout
}

UBO::~UBO()
{
  if(m_uniformIndices != NULL)
	  delete[] m_uniformIndices;

  if(m_uniformOffsets != NULL)
	  delete[] m_uniformOffsets;

  if(m_arrayStrides != NULL)
	  delete[] m_arrayStrides;

  if(m_matrixStrides != NULL)
	  delete[] m_matrixStrides;

  if(m_bufferData != NULL)
	  delete[] m_bufferData;
}

void UBO::createBuffer(Shader *shader, const GLuint bufferSize, const GLuint uniformCount, GLenum usage, const char **uniformNames)//shared layout
{
  glGenBuffers( 1, &m_bufferIndex );
	glBindBuffer( GL_UNIFORM_BUFFER, m_bufferIndex );
	glBufferData( GL_UNIFORM_BUFFER, this->m_bufferSize, NULL, usage );

  m_uniformIndices  = new GLuint[this->m_uniformCount];
	m_uniformOffsets  = new GLint[this->m_uniformCount];
	m_arrayStrides    = new GLint[this->m_uniformCount];
	m_matrixStrides   = new GLint[this->m_uniformCount];
	m_bufferData      = new unsigned char[bufferSize];

	glGetUniformIndices  ( shader->program, this->m_uniformCount, this->m_uniformNames, m_uniformIndices);
	glGetActiveUniformsiv( shader->program, this->m_uniformCount, m_uniformIndices, GL_UNIFORM_OFFSET,        m_uniformOffsets);
	glGetActiveUniformsiv( shader->program, this->m_uniformCount, m_uniformIndices, GL_UNIFORM_ARRAY_STRIDE,  m_arrayStrides);
	glGetActiveUniformsiv( shader->program, this->m_uniformCount, m_uniformIndices, GL_UNIFORM_MATRIX_STRIDE, m_matrixStrides);//maybe useless?!
}

void UBO::createBuffer(const GLuint bufferSize, const GLuint uniformCount, GLenum usage)//std140 layout
{
  m_uniformIndices = NULL;
  m_arrayStrides = NULL;
  m_matrixStrides = NULL;

  glGenBuffers( 1, &m_bufferIndex );
	glBindBuffer( GL_UNIFORM_BUFFER, m_bufferIndex );
	glBufferData( GL_UNIFORM_BUFFER, this->m_bufferSize, NULL, usage );

  m_bufferData      = new unsigned char[this->m_bufferSize];
  m_uniformOffsets  = new GLint[1];
  m_uniformOffsets[0] = 0;//data is already aligned for std layout
}

void UBO::setData(unsigned char *data, unsigned int offset, const GLuint size )
{
  memcpy( m_bufferData + offset, data, size );
}

void UBO::uploadData() const
{
	glBindBuffer( GL_UNIFORM_BUFFER, m_bufferIndex );
	glBufferSubData( GL_UNIFORM_BUFFER, m_uniformOffsets[0], m_bufferSize, m_bufferData );
	glBindBuffer( GL_UNIFORM_BUFFER, 0);
}

void UBO::setBindingPoint(GLuint bindingPoint)
{
  m_bufferBindingPoint = bindingPoint;
}

void UBO::bindBuffer() const
{
	glBindBufferBase( GL_UNIFORM_BUFFER, m_bufferBindingPoint, m_bufferIndex );
}

void UBO::unBindBuffer() const
{
  glBindBufferBase( GL_UNIFORM_BUFFER, m_bufferBindingPoint, 0 );
}