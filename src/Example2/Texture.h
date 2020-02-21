#pragma once

#include <string>
//#include "GLFW\glfw3.h"

class Texture
{
public:
	Texture() {}
	Texture(std::string _texturePath);
	
	bool loadTexture(const char * texturePath = 0);

public:
	unsigned int id_texture = 0;


private:
	std::string _texturePath;

	int _width = 0;
	int _height = 0;
};
