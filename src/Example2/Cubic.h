#pragma once

#include "DefaultRenderObject.h"
#include <string>
#include "glm\glm.hpp"
#include "Material.h"

class Cubic:public DefaultRenderObject
{
public:
	Cubic();

	/*
	void render(glm::mat4 transform);

	
	void setModelMatrix(glm::mat4 model)
	{
		_renderobj.setModelMatrix(model);
	}
	*/

	/**
	Render object.
	INPUT: transformation matrix, projection_matrix * view_matrix;
	*/
	//virtual void render(glm::mat4 transform = glm::mat4(1.0));
	

private:
	//template<namespace T>
	void _array_copy(float* cf, float* ct, int num)
	{
		for (int i = 0; i < num; ++i)
		{
			ct[i] = cf[i];
		}
	}

	void _array_copy(unsigned int* cf, unsigned int* ct, int num)
	{
		for (int i = 0; i < num; ++i)
		{
			ct[i] = cf[i];
		}
	}

private:
	//DefaultRenderObject _renderobj;

	float _vertexes[72];	
	float _normals[72];
	float _texcoord[48];
	unsigned int _indexes[36];

	int _num_vertexes = 24;		//4 * 6
	int _num_triangles = 12;	//2*6

	//std::string _texture_path;
	
	//DiffuseMaterial _material;
};