#include "Texture.h"
#include <Assets/Database.h>

using namespace Core::Assets;

Texture::Texture()
{
	Id = 0;
}


Texture::Texture(const Texture& t)
{
	Id = t.Id;
}


Texture::Texture(int width, int height, GLfloat filter, const unsigned char* bytes)
{
	Id = 0;
	
	CreateTexture(bytes, width, height, GL_RGBA, GL_RGBA, GL_CLAMP_TO_EDGE, filter, filter);
}


Texture::~Texture()
{
	glDeleteTextures(1, &Id);
}


GLuint Texture::GetID() const
{
	return Id;
}

	
void Texture::CreateTexture(bool depth, int width, int height)
{
	// Clear old texture if it exists
	glDeleteTextures(1, &Id);

	// Build new texture
	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_2D, Id);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		depth ? GL_DEPTH_COMPONENT32 : GL_RGBA32F,
		width,
		height,
		0,
		depth ? GL_DEPTH_COMPONENT : GL_RGBA,
		GL_UNSIGNED_BYTE,
		NULL
		);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::CreateTexture(const unsigned char* bytes, int width, int height, GLint iformat, GLint format, GLfloat wrap, GLfloat mag_filter, GLfloat min_filter)
{
	// Clear old texture if it exists
	glDeleteTextures(1, &Id);

	// Build new texture
	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_2D, Id);

	// Add Anisotropic Filtering
	GLfloat maxAniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		iformat,
		width,
		height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		bytes
		);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::CreateCubeMap(bool depth, int width, int height)
{
	// Clear old texture if it exists
	glDeleteTextures(1, &Id);

	// Build new texture
	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Id);

	Debug->GLError("Error generating CubeMap texture.");

	// Create all faces
	for (unsigned int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 
			depth ? GL_DEPTH_COMPONENT24 : GL_RGBA,
			width, height, 0, 
			depth ? GL_DEPTH_COMPONENT : GL_RGBA, 
			depth ? GL_FLOAT : GL_UNSIGNED_BYTE, 
			0);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
	Debug->GLError("Error setting CubeMap texture parameters.");

	if (depth)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		Debug->GLError("Error setting up depth CubeMap texture.");
	}
			
	// Unbind
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	Debug->GLError("Error creating CubeMap texture.");
}

/*
void Texture::LoadFromPNG(const std::string& pngFilename, GLuint width, GLuint height, GLfloat mag_filter, GLfloat min_filter)
{
	std::vector<unsigned char> image;

	unsigned error = LibTexture::LoadPNG(image, width, height, "Resources/Textures/" + pngFilename + ".png");

	// If there's an error, display it.
	if (error != 0)
	{
		Debug->Error("Error loading png texture file: " + std::to_string(error));
	}

	CreateTexture(&image[0], width, height, GL_SRGB_ALPHA, GL_RGBA, GL_CLAMP_TO_EDGE, mag_filter, min_filter);
}

	
void Texture::LoadSkyboxFromPNG(const std::string& pngFilename, GLuint width, GLuint height, GLfloat mag_filter, GLfloat min_filter)
{
	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Id);
		
	char suffix[] = { 'R', 'L', 'U', 'D', 'F', 'B'};
	for (unsigned int i = 0; i < 6; i++)
	{
		std::vector<unsigned char> image;

		std::string filename = "Resources/Textures/" + pngFilename + "_" + std::string(1, suffix[i]) + ".png";
		unsigned error = LibTexture::LoadPNG(image, width, height, filename);

		// If there's an error, display it.
		if (error != 0)
		{
			Debug->Error("Error loading png texture file: " + std::to_string(error));
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	// Unbind
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	Debug->GLError("Error creating CubeMap texture.");
}
*/