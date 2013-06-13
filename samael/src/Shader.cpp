#include "stdafx.h"
#include "Shader.h"
#include "Logger.h"

Shader::Shader(const char *vertexFilename, 
			         const char *fragmentFilename, 
			         const char *geometryFilename, 
			         const char *tesselationCTRLFilename, 
               const char *tesselationEVALFilename,
               const char *computeFilename,
               std::vector<std::string> &dynamicDefines)
{
	bool errorTrigger = true;
	std::string source;

  if(computeFilename != NULL)//compute shader
  {
    computeShader = glCreateShader(GL_COMPUTE_SHADER);
    if(!loadShaderSource(computeFilename, source, dynamicDefines))
    {
		  glDeleteShader(computeShader);
      return;
    }

    const char *src = source.c_str();
    glShaderSource( computeShader, 1, &src, NULL );
	  source.clear();

    glCompileShader(computeShader);

    if(!checkShaderStatus(computeShader, computeFilename))
	  {
		  glDeleteShader(computeShader);
      return;
    }

    program = glCreateProgram();
    glAttachShader(program, computeShader);
    glDeleteShader(computeShader);
  }
  else//no compute shader
  {
	  if(vertexFilename!=NULL)
	  {
		  vertexShader = glCreateShader(GL_VERTEX_SHADER);
		  if(!loadShaderSource(vertexFilename, source, dynamicDefines))
      {
			  errorTrigger = false;
      }

		  const char *src = source.c_str();
		  glShaderSource(vertexShader, 1, &src, NULL);
		  source.clear();
	  }

	  if(fragmentFilename != NULL)
	  {
		  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		  if(!loadShaderSource(fragmentFilename, source, dynamicDefines))
      {
			  errorTrigger = false;
      }

		  const char *src = source.c_str();
		  glShaderSource(fragmentShader, 1, &src, NULL);
		  source.clear();
	  }

	  if(geometryFilename != NULL)
	  {
		  geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		  if(!loadShaderSource(geometryFilename, source, dynamicDefines))
      {
			  errorTrigger = false;
      }

		  const char *src = source.c_str();
		  glShaderSource(geometryShader, 1, &src, NULL);
		  source.clear();
	  }

	  if(tesselationEVALFilename != NULL)
	  {
		  tesselationControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
		  tesselationEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);

		  if(!loadShaderSource(tesselationCTRLFilename, source, dynamicDefines))
      {
			  errorTrigger = false;
      }

		  const char *src = source.c_str();
		  glShaderSource(tesselationControlShader, 1, &src, NULL);
		  source.clear();

		  if(!loadShaderSource(tesselationEVALFilename, source, dynamicDefines))
			  errorTrigger=false;

		  src=source.c_str();
		  glShaderSource(tesselationEvaluationShader, 1, &src, NULL);
		  source.clear();
	  }

	  if(!errorTrigger)
	  {
		  glDeleteShader(vertexShader);
		  glDeleteShader(fragmentShader);
		  glDeleteShader(geometryShader);
		  glDeleteShader(tesselationControlShader);
		  glDeleteShader(tesselationEvaluationShader);

		  return;
	  }

	  glCompileShader(vertexShader);
	  if(fragmentFilename != NULL)
    {
		  glCompileShader(fragmentShader);
    }
	  if(geometryFilename != NULL)
    {
		  glCompileShader(geometryShader);
    }
	  if(tesselationEVALFilename != NULL)
	  {
		  glCompileShader(tesselationControlShader);
		  glCompileShader(tesselationEvaluationShader);
	  }

	  if(!checkShaderStatus(vertexShader, vertexFilename))
    {
		  errorTrigger = false;
    }

	  if(fragmentFilename != NULL && !checkShaderStatus(fragmentShader,fragmentFilename))
    {
		  errorTrigger = false;
    }

	  if(geometryFilename != NULL && !checkShaderStatus(geometryShader,geometryFilename))
    {
		  errorTrigger = false;
    }

	  if(tesselationEVALFilename != NULL && !checkShaderStatus(tesselationEvaluationShader, tesselationEVALFilename) && !checkShaderStatus(tesselationControlShader, tesselationCTRLFilename))
    {
		  errorTrigger = false;
    }

	  if(!errorTrigger)
	  {
		  glDeleteShader(vertexShader);
		  glDeleteShader(fragmentShader);
		  glDeleteShader(geometryShader);
		  glDeleteShader(tesselationEvaluationShader);
		  glDeleteShader(tesselationControlShader);

		  return;
	  }

	  program = glCreateProgram();

	  glAttachShader(program,vertexShader);
	  if(fragmentFilename!=NULL)
    {
		  glAttachShader(program,fragmentShader);
    }
	  if(geometryFilename!=NULL)
    {
		  glAttachShader(program,geometryShader);
    }
	  if(tesselationEVALFilename!=NULL)
	  {
		  glAttachShader(program, tesselationControlShader);
		  glAttachShader(program, tesselationEvaluationShader);
	  }

	  glDeleteShader(vertexShader);
	  if(fragmentFilename != NULL)
    {
		  glDeleteShader(fragmentShader);
    }
	  if(geometryFilename != NULL)
    {
		  glDeleteShader(geometryShader);
    }
	  if(tesselationEVALFilename != NULL)
	  {
		  glDeleteShader(tesselationControlShader);
		  glDeleteShader(tesselationEvaluationShader);
	  }
  }
  //equal for normal and compute shader
	glLinkProgram(program);

	GLint testVal;
	glGetProgramiv(program, GL_LINK_STATUS,&testVal);

	if(testVal == GL_FALSE)
	{
		char errorLog[1024];
		glGetProgramInfoLog(program, 1024, NULL, errorLog);
    if(vertexFilename != NULL)
    {
      QLOG_ERROR_NOCONTEXT() << "Error linking shader program " << vertexFilename <<  " because of:\n "<< errorLog;
    }
    else
    {
      QLOG_ERROR_NOCONTEXT() << "Error linking shader program " << computeFilename << " because of:\n "<< errorLog;
    }
		glDeleteProgram(program);
	}
}

Shader& Shader::operator=(const Shader& o)
{
  m_uniformLocations = o.m_uniformLocations;
  m_uniformBlockLocations = o.m_uniformBlockLocations;
  m_shaderStorageBlockLocations = o.m_shaderStorageBlockLocations;

	program = o.program;
	vertexShader = o.vertexShader;
  fragmentShader = o.fragmentShader;
  geometryShader = o.geometryShader;
  tesselationControlShader = o.tesselationControlShader;
  tesselationEvaluationShader = o.tesselationEvaluationShader;
  computeShader = o.computeShader;

    return *this;
}

Shader::~Shader()
{
	glDeleteProgram(program);
  m_uniformLocations.clear();
  m_uniformBlockLocations.clear();
  m_shaderStorageBlockLocations.clear();
}

void Shader::setUniform(int location, int type, const GLfloat* val) const
{
  switch(type)
  {
  case GL_FLOAT:
	  glUniform1f(location,val[0]);
	  break;
  case GL_FLOAT_VEC2:
	  glUniform2f(location,val[0],val[1]);
	  break;
  case GL_FLOAT_VEC3:
	  glUniform3f(location,val[0],val[1],val[2]);
	  break;
  case GL_FLOAT_VEC4:
	  glUniform4f(location,val[0],val[1],val[2],val[3]);
	  break;
  case GL_FLOAT_MAT2:
	  glUniformMatrix2fv(location,1,GL_FALSE,val);
	  break;
  case GL_FLOAT_MAT3:
	  glUniformMatrix3fv(location,1,GL_FALSE,val);
	  break;
  case GL_FLOAT_MAT4:
	  glUniformMatrix4fv(location,1,GL_FALSE,val);
	  break;
  }
}

void Shader::setUniform(int location, int type, const GLuint* val) const
{
  switch(type)
	{
	case GL_UNSIGNED_INT:
		glUniform1ui(location,val[0]);
		break;
	case GL_UNSIGNED_INT_VEC2:
		glUniform2ui(location,val[0],val[1]);
		break;
	case GL_UNSIGNED_INT_VEC3:
		glUniform3ui(location,val[0],val[1],val[2]);
		break;
	case GL_UNSIGNED_INT_VEC4:
		glUniform4ui(location,val[0],val[1],val[2],val[3]);
		break;
	}
}

void Shader::setUniform(int location, int type, const GLint* val) const
{
	switch(type)
	{
	case GL_INT:
		glUniform1i(location,val[0]);
		break;
	case GL_INT_VEC2:
		glUniform2i(location,val[0],val[1]);
		break;
	case GL_INT_VEC3:
		glUniform3i(location,val[0],val[1],val[2]);
		break;
	case GL_INT_VEC4:
		glUniform4i(location,val[0],val[1],val[2],val[3]);
		break;
	}
}

void Shader::setTexture(int location, GLint slot) const
{
	glUniform1i( location, GL_TEXTURE0 + slot );
}

void Shader::enableTransformFeedback(int count, const char** varyings, GLenum buffertype) const
{
	glTransformFeedbackVaryings(program, count, varyings, buffertype);
	glLinkProgram(program);//new linking because some unused varyings could be in use now
	
	int length=0;
	GLsizei size=0;
	GLenum type=GL_V3F;
	char var[64];
	for(int i=0;i!=count;i++)
	{
		glGetTransformFeedbackVarying(program, i, 64, &length, &size, &type, var);
		if(strcmp(var,varyings[i]))
			QLOG_ERROR_NOCONTEXT() << "Error, the TransformFeedback varying "<< varyings[i] <<" is erroneous.";
	}
}

void Shader::dispatchComputeShader(GLuint workGroupNumberX, GLuint workGroupNumberY, GLuint workGroupNumberZ) const
{
  glDispatchCompute(workGroupNumberX, workGroupNumberY, workGroupNumberZ);
}

void Shader::dispatchComputeShaderIndirect(GLuint dispatchBuffer, GLuint offset) const
{
  glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, dispatchBuffer);
  glDispatchComputeIndirect(offset);
  glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, 0);
}

void Shader::useShader() const
{
	glUseProgram(program);
}

void Shader::useNoShader() const
{
	glUseProgram(0);
}

bool Shader::loadShaderSource(const char *filename, std::string &shadersource, std::vector<std::string> &dynamicDefines)
{
	std::ifstream file(filename);
	std::string line;

	if(file.is_open())
  {
		while(!file.eof())
		{
			std::getline(file, line);
			line += '\n';

      for(int i = 0; i < dynamicDefines.size(); i++)
      {
        size_t pos = line.find(dynamicDefines[i]);
        if(pos != std::string::npos)
        {
          size_t defineOffset = dynamicDefines[i].find(" ");
          line.insert(pos + defineOffset, dynamicDefines[i], defineOffset, std::string::npos);
          dynamicDefines.erase(dynamicDefines.begin() + i);
        }
      }

			shadersource += line;
		}
  }
	else
	{
		file.close();

		QLOG_ERROR_NOCONTEXT() << "Error: couldn't open shader source file "<<filename<<".";

		return false;
	}

	file.close();
	return true;
}

bool Shader::checkShaderStatus(GLuint shader, const char *filename) const
{
	GLint testVal;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &testVal);

	if(testVal==GL_FALSE)
	{
		char errorLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, errorLog);
		QLOG_ERROR_NOCONTEXT()<<"Error compiling "<<filename<<" because of "<<errorLog;
		return false;
	}

	return true;
}
