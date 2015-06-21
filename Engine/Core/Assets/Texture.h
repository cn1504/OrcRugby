#pragma once
#include <Core.h>

namespace Core
{
	namespace Assets
	{

		class Texture
		{
		private:
			GLuint Id;

		public:
			Texture();
			Texture(const Texture& t);
			Texture(int width, int height, GLfloat filter, const unsigned char* bytes);
			~Texture();

			GLuint GetID() const;

			void CreateTexture(bool depth, int width, int height);
			void CreateTexture(const unsigned char* bytes, int width, int height, GLint iformat, GLint format, GLfloat wrap, GLfloat mag_filter, GLfloat min_filter);
			void CreateCubeMap(bool depth, int width, int height);
			void CreateFPTexture(int width, int height);
			void CreateMonoFPTexture(int width, int height);
			/*
			void LoadFromPNG(const std::string& pngFilename, GLuint width, GLuint height, GLfloat mag_filter = GL_LINEAR, GLfloat min_filter = GL_LINEAR);
			void LoadSkyboxFromPNG(const std::string& pngFilename, GLuint width, GLuint height, GLfloat mag_filter = GL_LINEAR, GLfloat min_filter = GL_LINEAR);		
			*/
		};

	}
}