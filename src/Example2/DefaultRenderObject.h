#pragma once
#include "Shader.h"
#include "glm/glm.hpp"
#include "Material.h"

/**
Render object. The class save the resource that the rendering of an object needed.
Including: Pointer of shader, vertexes, indexes and texture.

NOTE: a shader can be used by not only one objects, so the class only use the pointer of shader here for the comvinience of  reusage.

*/

class DefaultRenderObject
{
public:
	DefaultRenderObject();
	~DefaultRenderObject();

	void deleteResource();


	//virtual bool loadTexture(const char* texturePath);

	virtual void setVertexes(int num, float* pvertexes, bool dynamic_draw = false);
	virtual void setIndexes(int num_triangle, unsigned int* pindexes, bool dynamic_draw=false);
	//virtual void setTexture(int width, int height, unsigned int* ptexture, unsigned int num_components = 3, bool dynamic_draw=false);
	virtual void setTexcoord(float* ptexcoord, bool dynamic_draw = false);			// number of texcoord is the same as vertex number
	virtual void setNormal(float* pnormal, bool dynamic_draw = false);
	virtual void setTextureUnit(unsigned int id_unit);

	virtual void setModelMatrix(glm::mat4 model);

	virtual void setShader(Shader* shader)
	{
		pshader = shader;
	}
	virtual Shader* getShader()
	{
		if (!pshader)
		{
			pshader = new Shader("default.vs", "default.fs");
		}

		return pshader;
	}

	/**
	Bind vertexes, indexes and texcoord to vao, and set attrib-pointer.
	Bind texture to texture unit.
	*/
	virtual void bindAll();

	/**
	Render object.
	INPUT: transformation matrix, projection_matrix * view_matrix;
	*/
	virtual void render(glm::mat4 transform=glm::mat4(1.0));

	DiffuseMaterial& getMaterial()
	{
		return _material;
	}

	void useTexture(bool use_tex = true)
	{
		_use_texture = use_tex;
	}


	void getSizeRange(glm::vec3& sizeMin, glm::vec3& sizeMax)
	{
		//sizeMin = glm::vec3(99999999.0, 99999999.0, 9999999.0);
		//sizeMax = glm::vec3(-99999999.0, -99999999.0, -9999999.0);

		float xMin = 999999.0, yMin = 99999999.0, zMin = 99999999.0;
		float xMax = -999999.0, yMax = -99999999.0, zMax = -99999999.0;

		for (int i = 0; i < _num_vertexes; ++i)
		{
			float * p = _pvertexes + 3 * i;
			
			xMin = xMin > p[0] ? p[0] : xMin;
			xMax = xMax < p[0] ? p[0] : xMax;

			yMin = yMin > p[1] ? p[1] : yMin;
			yMax = yMax < p[1] ? p[1] : yMax;

			zMin = zMin > p[2] ? p[2] : zMin;
			zMax = zMax < p[2] ? p[2] : zMax;
		}

		sizeMin = glm::vec3(xMin, yMin, zMin);
		sizeMax = glm::vec3(xMax, yMax, zMax);
	}

public:
	Shader* pshader=0;

protected:
	unsigned int _id_vao = 0;

	// vertexes 
	int _num_vertexes;
	float* _pvertexes = 0;
	unsigned int _id_vertex = 0;


	// indexes
	int _num_triangles;						// number of triangle faces. Number of indexes should be: _num_triangles*3
	unsigned int* _pindexes=0;
	unsigned int _id_index = 0;

	
	// texture
	//int _tex_width, _tex_height;
	//unsigned int * _ptexture=0;
	//unsigned int _id_texture_data = 0;
	bool _use_texture = true;
	unsigned int _id_texture_unit = 0;
	DiffuseMaterial _material;
	SimpleMaterial _simple_material;

	// texture coordination
	float* _ptexcoord = 0;
	unsigned int _id_texcoord = 0;

	// normal
	float* _pnormal = 0;
	unsigned int _id_normal = 0;


	glm::mat4 _mat_model;
};