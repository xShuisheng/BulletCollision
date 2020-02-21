#include "Texture.h"

//#include "GLFW\glfw3.h"
#include "glad\glad.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(std::string path)
{
	_texturePath = path;
}

bool Texture::loadTexture(const char * texturePath)
{
	if (texturePath)
	{
		_texturePath = std::string(texturePath);
	}

	glGenTextures(1, &id_texture);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(_texturePath.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, id_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		_width = width;
		_height = height;

		return true;
	}
	else
	{
		std::cout << "Texture failed to load at path: " << _texturePath.c_str() << std::endl;
		stbi_image_free(data);

		return false;
	}
}
