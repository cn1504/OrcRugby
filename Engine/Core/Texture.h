#pragma once
#include "Core.h"

namespace Core
{

	class Texture
	{
	private:
		GLuint Id;

	public:
		Texture();
		Texture(const Texture& t);
		~Texture();

		GLuint GetID();
			
		void CreateTexture(bool depth, int width, int height);
		void CreateTexture(const unsigned char* bytes, int width, int height, GLint iformat, GLint format, GLfloat wrap, GLfloat mag_filter, GLfloat min_filter);
		void CreateCubeMap(bool depth, int width, int height);
		void LoadFromPNG(const std::string& pngFilename, GLuint width, GLuint height, GLfloat mag_filter = GL_LINEAR, GLfloat min_filter = GL_LINEAR);
	};

}