#include <iostream>
#include <stdarg.h>
#include <assert.h>

class Texture;

class Renderquad
{
public:

	Renderquad();
	~Renderquad();

	void setRenderTarget(int count, Texture *tex[]);
	void setRenderTarget(int count, ...);
	void render(bool direct);

private:

	Renderquad(const Renderquad& r){}
	Renderquad& operator=(const Renderquad& r){}

	unsigned int count;

	GLuint vaoIndex;
	GLuint vboindex,texIndex;
	GLuint depthTex;
	GLuint fboIndex;
	GLuint m_width,m_height;
};