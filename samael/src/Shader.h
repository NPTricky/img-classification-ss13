#ifndef SHADER_H_
#define SHADER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <stdarg.h>
#include <assert.h>
#include <vector>

class Shader
{
public:

  enum ShaderSlots
	{
		POSITION,
		TEXTURE0,
		TEXTURE1,
		TEXTURE2,
		TEXTURE3,
		NORMAL,
		BINORMAL,
		TANGENT,
		WEIGHTS,
		BONEINDICES,
		SPECIAL0,
		SPECIAL1,
		SPECIAL2,
		SPECIAL3
	};

  Shader(const char *vertexFilename, 
			   const char *fragmentFilename, 
			   const char *geometryFilename, 
			   const char *tesselationCTRLFilename, 
         const char *tesselationEVALFilename,
         const char *computeFilename,
         std::vector<std::string> &dynamicDefines = std::vector<std::string>());

  Shader& operator=(const Shader& o);

	~Shader();

	void setUniform(int location, int type, const GLfloat* val) const;
  void setUniform(int location, int type, const GLuint* val) const;
	void setUniform(int location, int type, const GLint* val) const;
	void setTexture(int location, GLint slot) const;

	void enableTransformFeedback(int count, const char** varyings, GLenum buffertype) const;
  void dispatchComputeShader(GLuint workGroupNumberX, GLuint workGroupNumberY, GLuint workGroupNumberZ) const;
  void dispatchComputeShaderIndirect(GLuint dispatchBuffer, GLuint offset) const;

	void useShader() const;
	void useNoShader() const;

	friend class UBO;

private:

	Shader(){}
	Shader(const Shader&){}

	bool loadShaderSource(const char *filename, std::string &shadersource, std::vector<std::string> &dynamicDefines);
	bool checkShaderStatus(GLuint shader, const char *filename) const;

	std::vector<int> m_uniformLocations;
  std::vector<int> m_uniformBlockLocations;
  std::vector<int> m_shaderStorageBlockLocations;

	GLuint program;
	GLuint vertexShader,fragmentShader,geometryShader,tesselationControlShader,tesselationEvaluationShader, computeShader;
};

#endif