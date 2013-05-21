#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <list>

class Texture
{
public:

	Texture(GLuint width, GLuint height, GLenum target, GLenum type, GLenum internalFormat, GLenum format, void* data = NULL);
  Texture& operator=(const Texture& o);
	~Texture();

	void setTexture(GLuint slot);
	void unsetTexture();

	void setTexParameters(GLint edgeModeS, GLint edgeModeT, GLint magFilter, GLint minFilter);

	cv::Vec<unsigned int, 2> getResolution();
	GLuint getSlot();
	GLenum getTarget();

	friend class Renderquad;
  friend class PBO;

private:

	Texture(const Texture&) {}

	GLsizei m_width, m_height;
	GLuint m_texIndex;
	GLenum m_target;
  GLenum m_internalFormat;
  GLenum m_format;
  GLenum m_type;
	GLuint m_slot;
};

#endif