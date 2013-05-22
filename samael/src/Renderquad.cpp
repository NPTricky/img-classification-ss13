#include "stdafx.h"
#include "Renderquad.h"
#include "Texture.h"

Renderquad::Renderquad()
{
	fboIndex = depthTex = -1;

	float v[] = {-1,-1, 1,-1, -1,1, 1,1};

	float texVec[] = {0,0, 1,0, 0,1, 1,1};

	glGenVertexArrays(1, &vaoIndex);
	glBindVertexArray(vaoIndex);

		glGenBuffers(1, &vboindex);
		glBindBuffer(GL_ARRAY_BUFFER, vboindex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &texIndex);
		glBindBuffer(GL_ARRAY_BUFFER, texIndex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(texVec), texVec, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	glBindVertexArray(0);	
}

Renderquad::~Renderquad()
{
	if(depthTex != -1)
  {
    glDeleteTextures(1, &depthTex);
  }

	if(fboIndex != -1)
  {
    glDeleteFramebuffers(1, &fboIndex);
  }
}


void Renderquad::setRenderTarget(int count, Texture *tex[])
{
	assert(count > 1);
	this->count = count;

	m_width  = tex[0]->getResolution()[0];
	m_height = tex[0]->getResolution()[1];

	if(depthTex == -1)
	{
		glGenTextures(1,&depthTex);
		glBindTexture(GL_TEXTURE_2D,depthTex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if(fboIndex == -1)
  {
    glGenFramebuffers(1,&fboIndex);
  }

	glBindFramebuffer(GL_FRAMEBUFFER,fboIndex);
		for(int i = 0; i != this->count; i++)
    {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tex[i]->m_texIndex, 0);
    }
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderquad::setRenderTarget(int count, ...)
{
	assert(count > 1);
	this->count = count;

	va_list texList;
	va_start(texList, count);

	Texture *tex = va_arg(texList, Texture*);

	m_width  = tex->getResolution()[0];
	m_height = tex->getResolution()[1];

	if(depthTex == -1)
	{
		glGenTextures(1,&depthTex);
		glBindTexture(GL_TEXTURE_2D, depthTex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if(fboIndex == -1)
  {
    glGenFramebuffers(1,&fboIndex);
  }

	glBindFramebuffer(GL_FRAMEBUFFER, fboIndex);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->m_texIndex, 0);
		for(int i = 1; i != this->count; i++)
		{
			tex = va_arg(texList, Texture*);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tex->m_texIndex, 0);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderquad::render(bool direct)
{
	if(!direct)
	{
		glBindFramebuffer(GL_FRAMEBUFFER,fboIndex);

		GLenum *fboBuffer = new GLenum[count];
		for(int i = 0; i != count; i++)
    {
			fboBuffer[i] = GL_COLOR_ATTACHMENT0 + i;
    }
		glDrawBuffers(count, fboBuffer);
		delete[] fboBuffer;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glBindVertexArray(vaoIndex);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if(!direct)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		GLenum windowBuff[] = { GL_FRONT_LEFT };
		glDrawBuffers(1, windowBuff);
	}
}