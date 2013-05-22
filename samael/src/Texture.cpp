#include "stdafx.h"
#include "Texture.h"

Texture::Texture(GLuint width, GLuint height, GLenum target, GLenum type, GLenum internalFormat, GLenum format, void* data) : m_target(target),
	                                                                                                                            m_width(width),
	                                                                                                                            m_height(height),
                                                                                                                              m_type(type),
                                                                                                                              m_format(format),
                                                                                                                              m_slot(0)
{
	glGenTextures(1,&m_texIndex);
		glBindTexture(m_target,m_texIndex);
		glTexParameteri(m_target,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(m_target,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(m_target,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(m_target,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

		glTexImage2D(m_target, 0, internalFormat, m_width, m_height, 0, m_format, m_type, data);
	glBindTexture(m_target, 0);
}

Texture& Texture::operator=(const Texture& o)
{
  m_width = o.m_width;
  m_height = o.m_height;
	m_texIndex = o.m_texIndex;
	m_target = o.m_target;
  m_internalFormat = o.m_internalFormat;
  m_format = o.m_format;
  m_type = o.m_type;
	m_slot = o.m_slot;

  return *this;
}

//
//Texture::Texture(const char* filename, GLenum target, bool streamed) : StreamableObject(1), m_target(target),
//                                                                                            m_slot(0)
//{
//	ILuint tex;
//	ilGenImages(1, &tex);
//	ilBindImage(tex);
//		ILboolean success = ilLoadImage((const wchar_t*)filename);
//		if(!success)
//			printf("ERROR, couldn't open file %s\n",filename);
//
//		//success = ilConvertImage(format,type);
//		//if(!success)
//		//	printf("ERROR, couldn't convert file %s\n",filename);
//      m_width = ilgetInteger(IL_IMAGE_WIDTH);
//      m_height = ilgetInteger(IL_IMAGE_HEIGHT);
//      m_type = ilgetInteger(IL_IMAGE_TYPE);
//      m_format = ilgetInteger(IL_IMAGE_FORMAT);
//
//      GLuint componentNumber = 0;
//      getImageInformations(m_width, m_height, m_internalFormat, m_format, m_type, componentNumber);
//
//      glGenTextures(1,&m_texIndex);
//      glBindTexture(m_target,m_texIndex);
//      glTexParameteri(m_target,GL_TEXTURE_WRAP_S,GL_REPEAT);
//	    glTexParameteri(m_target,GL_TEXTURE_WRAP_T,GL_REPEAT);
//	    glTexParameteri(m_target,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
//	    glTexParameteri(m_target,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
//
//      switch(m_type)
//      {
//      case GL_UNSIGNED_BYTE:
//        streamed ? setStream((GLubyte*)ilgetData(), componentNumber * m_width * m_height * sizeof(GLubyte)) : 
//          glTexImage2D( m_target, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, (GLubyte*)ilgetData() );
//        break;
//      case GL_BYTE:
//        streamed ? setStream((GLbyte*)ilgetData(), componentNumber * m_width * m_height * sizeof(GLbyte)) : 
//          glTexImage2D( m_target, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, (GLbyte*)ilgetData() );
//        break;
//      case GL_UNSIGNED_SHORT:
//        streamed ? setStream((GLushort*)ilgetData(), componentNumber * m_width * m_height * sizeof(GLushort)) : 
//          glTexImage2D( m_target, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, (GLushort*)ilgetData() );
//        break;
//      case GL_SHORT:
//        streamed ? setStream((GLshort*)ilgetData(), componentNumber * m_width * m_height * sizeof(GLshort)) : 
//          glTexImage2D( m_target, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, (GLshort*)ilgetData() );
//        break;
//      case GL_UNSIGNED_INT:
//        streamed ? setStream((GLuint*)ilgetData(), componentNumber * m_width * m_height * sizeof(GLuint)) : 
//          glTexImage2D( m_target, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, (GLuint*)ilgetData() );
//        break;
//      case GL_INT:
//        streamed ? setStream((GLint*)ilgetData(), componentNumber * m_width * m_height * sizeof(GLint)) : 
//          glTexImage2D( m_target, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, (GLint*)ilgetData() );
//        break;
//      case GL_FLOAT://float is default
//      default:
//        streamed ? setStream((GLfloat*)ilgetData(), componentNumber * m_width * m_height * sizeof(GLfloat)) : 
//          glTexImage2D( m_target, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, (GLfloat*)ilgetData() );
//      }
//    glBindTexture(m_target, 0);
//    
//  ilBindImage(0);
//  ilDeleteImages(1, &tex);
//}

Texture::~Texture()
{
	glDeleteTextures(1,&m_texIndex);
}

void Texture::setTexture(GLuint slot)
{
	if(slot > GL_TEXTURE31)
		printf("ERROR, texture slot too high %i\n",slot);

	m_slot = slot;
  
	glActiveTexture( GL_TEXTURE0 + m_slot );
	glBindTexture( m_target, m_texIndex );
}

void Texture::unsetTexture()
{
	glActiveTexture( GL_TEXTURE0 + m_slot );
	glBindTexture( m_target, 0 );
}

void Texture::setTexParameters(GLint edgeModeS, GLint edgeModeT, GLint magFilter, GLint minFilter)
{
	glBindTexture(m_target,m_texIndex);
		glTexParameteri(m_target,GL_TEXTURE_WRAP_S,edgeModeS);
		glTexParameteri(m_target,GL_TEXTURE_WRAP_T,edgeModeT);
		glTexParameteri(m_target,GL_TEXTURE_MAG_FILTER,magFilter);
		glTexParameteri(m_target,GL_TEXTURE_MIN_FILTER,minFilter);
	glBindTexture(m_target,0);
}

cv::Vec<unsigned int,2> Texture::getResolution()
{
	return cv::Vec<unsigned int,2>(m_width,m_height);
}

GLuint Texture::getSlot()
{
	return m_slot;
}

GLenum Texture::getTarget()
{
	return m_target;
}

//void Texture::getImageInformations(GLsizei& width, GLsizei& height, GLenum& internalFormat, GLenum& format, GLenum& type, GLuint& components)
//{
//  width = ilgetInteger(IL_IMAGE_WIDTH);
//  height = ilgetInteger(IL_IMAGE_HEIGHT);
//
//  switch(ilgetInteger(IL_IMAGE_FORMAT))
//  {
//  case IL_RGB:
//    format = GL_RGB;
//    break;
//  case IL_RGBA:
//    format = GL_RGBA;
//    break;
//  case IL_BGR:
//    format = GL_BGR;
//    break;
//  case IL_BGRA:
//    format = GL_BGRA;
//    break;
//  default:
//    format = GL_RGBA;
//  }
//
//  switch(m_format)
//  {
//  case GL_RGB:
//  case GL_BGR:
//    components = 3;
//    switch(ilgetInteger(IL_IMAGE_BITS_PER_PIXEL))
//    {
//    case 8:
//      internalFormat = GL_RGB8;
//      break;
//    case 16:
//      internalFormat = GL_RGB16;
//      break;
//    case 32:
//      internalFormat = GL_RGB32F;
//      break;
//    default:
//      internalFormat = GL_RGB8;
//    }
//    break;
//  case GL_RGBA:
//  case GL_BGRA:
//    components = 4;
//    switch(ilgetInteger(IL_IMAGE_BITS_PER_PIXEL))
//    {
//    case 8:
//      internalFormat = GL_RGBA8;
//      break;
//    case 16:
//      internalFormat = GL_RGBA16;
//      break;
//    case 32:
//      internalFormat = GL_RGBA32F;
//      break;
//    default:
//      internalFormat = GL_RGBA8;
//    }
//    break;
//  }
//
//  switch(ilgetInteger(IL_IMAGE_TYPE))
//  {
//  case IL_UNSIGNED_BYTE:
//    type = GL_UNSIGNED_BYTE;
//    break;
//  case IL_BYTE:
//    type = GL_BYTE;
//    break;
//  case IL_UNSIGNED_SHORT:
//    type = GL_UNSIGNED_SHORT;
//    break;
//  case IL_SHORT:
//    type = GL_SHORT;
//    break;
//  case IL_UNSIGNED_INT:
//    type = GL_UNSIGNED_INT;
//    break;
//  case IL_INT:
//    type = GL_INT;
//    break;
//  case IL_FLOAT:
//  default:
//    type = GL_FLOAT;
//  }
//}