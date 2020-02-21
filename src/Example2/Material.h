#pragma once

#include "glm\glm.hpp"
//#include "Texture.h"

struct SimpleMaterial
{
	glm::vec3 specular = glm::vec3(0.1, 0.1, 0.1);
	glm::vec3 diffuse = glm::vec3(0.5, 0.5, 0.5);
	float shininess = 32.0;
};

//struct SpecularMaterial
//{
//	Texture specular;
//	glm::vec3 diffuse = glm::vec3(0.5, 0.5, 0.5);
//	float shininess = 32.0;
//};
//
//struct DiffuseMaterial
//{
//	glm::vec3 specular=glm::vec3(0.1,0.1,0.1);
//	Texture diffuse;
//	float shininess=32.0;
//};
//
//struct Material
//{
//	Texture specular;
//	Texture diffuse;
//	float shininess=32.0;
//};