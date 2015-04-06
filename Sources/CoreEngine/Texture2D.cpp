
#include <iostream>

#include "Texture2D.h"

Texture2D::Texture2D(int width, int height,
	bool generateMipmaps,
	GLenum internalFormat,
	GLenum format,
	GLenum dataType)
	:width(width), height(height), generateMipmaps(generateMipmaps), internalFormat(internalFormat), format(format), dataType(dataType)
{
	glGenTextures(1, &texture_id);
	//glBindTexture(GL_TEXTURE_2D, texture_id);
	if (generateMipmaps)
	{
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, this->width, this->height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		texture_min_filter = GL_NEAREST_MIPMAP_LINEAR;
		texture_mag_filter = GL_LINEAR_MIPMAP_LINEAR;
	}
	else
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		texture_min_filter = GL_NEAREST;
		texture_mag_filter = GL_LINEAR;
	}

	glBindTexture(GL_TEXTURE_2D, texture_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_mag_filter);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrap_t);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, texture_enviroment);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(const GLvoid* pixels,
	int width, int height,
	bool generateMipmaps,
	GLenum internalFormat,
	GLenum format,
	GLenum dataType)
:Texture2D(width, height, generateMipmaps, internalFormat, format, dataType)
{
	setPixels(pixels);
}

/*Texture2D::Texture2D(const char* filename, bool generateMipmaps)
: generateMipmaps(generateMipmaps)
{
	Image* image = loadBMP(filename);
	
	if (image != NULL)
	{
		width = image->width;
		height = image->height;

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		if (generateMipmaps)
		{
			//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, this->width, this->height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
			texture_min_filter = GL_NEAREST_MIPMAP_LINEAR;
			texture_mag_filter = GL_LINEAR_MIPMAP_LINEAR;
		}
		else
		{
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
			texture_min_filter = GL_NEAREST;
			texture_mag_filter = GL_LINEAR;
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		setPixels(image->pixels);
	}
	else
		std::cout << "Texture2D: Error occurred while reading file: " << filename << std::endl;

	delete image;
}*/

Texture2D::Texture2D(const GLvoid* pixels,
	int region[],
	int width, int height,
	bool generateMipmaps,
	GLenum internalFormat,
	GLenum format,
	GLenum dataType)
	:Texture2D(width, height, generateMipmaps, internalFormat, format, dataType)
{
	setPixels(pixels);

	textures = new TextureRegion[1];

	textures[0].u_v[0] = (float)region[0] / width;
	textures[0].u_v[1] = (float)region[1] / height;

	textures[0].u_v[2] = (float)region[2] / width;
	textures[0].u_v[3] = (float)region[1] / height;

	textures[0].u_v[4] = (float)region[2] / width;
	textures[0].u_v[5] = (float)region[3] / height;

	textures[0].u_v[6] = (float)region[0] / width;
	textures[0].u_v[7] = (float)region[3] / height;
}

Texture2D::~Texture2D()
{
	std::cout << "Texture2D ~~~deleted~~~" << std::endl;
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glDeleteTextures(1, &texture_id);
	delete[] textures;
}

void Texture2D::setPixels(const GLvoid* pixels)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
	if (generateMipmaps)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, this->width, this->height, format, dataType, pixels);
	}
	else
	{
		if (pixelsSet)
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->width, this->height, format, dataType, pixels);
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, this->width, this->height, 0, format, dataType, pixels);
			pixelsSet = true;
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

//TODO implement reading by offset
char* Texture2D::getPixels(GLint x, GLint y, GLint _width, GLint _height)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);

	//TEMPPPPPPPPP
	//TODO find solution for flexibility
	int color_componens;
	switch (format)
	{
	case GL_RED: case GL_GREEN: case GL_BLUE: case GL_ALPHA: case GL_LUMINANCE:
		color_componens = 1;
		break;
	case GL_RGB: case GL_BGR_EXT:
		color_componens = 3;
	case GL_RGBA: case GL_BGRA_EXT:
		color_componens = 4;
		break;
	default:
		color_componens = 3;
		break;
	}

	char* pixels = new char[width*height*color_componens];
	glGetTexImage(GL_TEXTURE_2D, 0, format, dataType, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	return pixels;
}

char* Texture2D::getPixels()
{
	return getPixels(0, 0, width, height);
}