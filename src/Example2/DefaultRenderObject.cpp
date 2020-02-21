#include "DefaultRenderObject.h"
#include "stb_image.h"
#include "glad\glad.h"
#include <iostream>

DefaultRenderObject::DefaultRenderObject()
{
	_mat_model = glm::mat4(1.0);

	//_simple_material

	glGenVertexArrays(1, &_id_vao);

	int error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "gen vao ERROR:    " << error << std::endl;
	}
}

DefaultRenderObject::~DefaultRenderObject()
{
	

}

void DefaultRenderObject::deleteResource()
{
	if (_id_vertex)
	{
		glDeleteBuffers(1, &_id_vertex);
	}

	if (_id_normal)
	{
		glDeleteBuffers(1, &_id_normal);
	}


	if (_id_texcoord)
	{
		glDeleteBuffers(1, &_id_texcoord);
	}

	if (_id_vao)
	{
		glDeleteVertexArrays(1, &_id_vao);
	}
}


void DefaultRenderObject::setVertexes(int num, float * pvertexes, bool dynamic_draw)
{
	_pvertexes = pvertexes;
	_num_vertexes = num;

	if (_id_vertex == 0)
	{
		glGenBuffers(1, &_id_vertex);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _id_vertex);
	glBufferData(GL_ARRAY_BUFFER, 3 * _num_vertexes * sizeof(float), _pvertexes, dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	int error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "IN setVertex GL ERROR:    " << error << std::endl;
	}

}

void DefaultRenderObject::setIndexes(int num_triangle, unsigned int * pindexes, bool dynamic_draw)
{
	_pindexes = pindexes;
	_num_triangles = num_triangle;

	if (_id_index == 0)
	{
		glGenBuffers(1, &_id_index);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * _num_triangles * sizeof(unsigned int), _pindexes, dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	int error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "IN setVertex GL ERROR:    " << error << std::endl;
	}
}



void DefaultRenderObject::setTexcoord(float * ptexcoord, bool dynamic_draw)
{
	_ptexcoord = ptexcoord;

	if (_id_texcoord == 0)
	{
		glGenBuffers(1, &_id_texcoord);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _id_texcoord);
	glBufferData(GL_ARRAY_BUFFER, 2 * _num_vertexes * sizeof(float), _ptexcoord, dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);		// texcoord only has two dimensions

	int error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "IN setVertex GL ERROR:    " << error << std::endl;
	}
}

void DefaultRenderObject::setNormal(float * pnormal, bool dynamic_draw)
{
	_pnormal = pnormal;

	if (_id_normal == 0)
	{
		glGenBuffers(1, &_id_normal);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _id_normal);
	glBufferData(GL_ARRAY_BUFFER, 3 * _num_vertexes * sizeof(float), _pnormal, dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	int error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "IN setVertex GL ERROR:    " << error << std::endl;
	}
}

void DefaultRenderObject::setTextureUnit(unsigned int id_unit)
{
	_id_texture_unit = id_unit;
}

void DefaultRenderObject::setModelMatrix(glm::mat4 model)
{
	_mat_model = model;
}

void DefaultRenderObject::bindAll()
{

	int error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "here ERROR:    " << error << std::endl;

	}

	if (_id_vao == 0)
	{
		glGenVertexArrays(1, &_id_vao);
	}

	glBindVertexArray(_id_vao);


	error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "here ERROR:    " << error << std::endl;
	}

	// vertex
	if (_id_vertex)
	{

		glBindBuffer(GL_ARRAY_BUFFER, _id_vertex);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
	}

	error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "here ERROR:    " << error << std::endl;
	}

	// normal
	if (_id_normal)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id_normal);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
	}

	error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "here ERROR:    " << error << std::endl;
	}

	// texcoord
	if (_id_texcoord)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id_texcoord);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(2);
	}

	error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "here ERROR:    " << error << std::endl;
	}

	if (_id_index)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id_index);
	}

	error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "here ERROR:    " << error << std::endl;
	}
}

void DefaultRenderObject::render(glm::mat4 transform)
{
	if (!pshader)
	{
		pshader = new Shader("default.vs","default.fs");
	}

	int error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "-1GL ERROR:    " << error << std::endl;
	}

	pshader->use();

	error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "0GL ERROR:    " << error << std::endl;
	}
	
	if (_use_texture)
	{
		pshader->setVec3("material.specular", _material.specular);
		pshader->setInt("material.diffuse", _id_texture_unit);
		pshader->setFloat("material.shininess", _material.shininess);
		//pshader->setVec3("material.diffuse", 0.1, 0.6, 0.1);
	}
	else
	{
		pshader->setVec3("material.specular", _simple_material.specular);
		pshader->setVec3("material.diffuse", _simple_material.diffuse);
		pshader->setFloat("material.shininess", _material.shininess);
	}

	error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "1GL ERROR:    " << error << std::endl;
	}


	pshader->setMat4("model", _mat_model);
	pshader->setMat4("transform", transform);

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "2GL ERROR:    " << error << std::endl;
	}

	if (_use_texture)
	{
		glActiveTexture(GL_TEXTURE0 + _id_texture_unit);
		glBindTexture(GL_TEXTURE_2D, _material.diffuse.id_texture);
	}

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "3GL ERROR:    " << error << std::endl;
	}


	glBindVertexArray(_id_vao);

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "4GL ERROR:    " << error << std::endl;
	}


	if (_id_index != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id_index);
		glDrawElements(GL_TRIANGLES, 3 * _num_triangles, GL_UNSIGNED_INT, 0);

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cout << "4GL ERROR:    " << error << std::endl;
		}
		//glDrawArrays(GL_TRIANGLES, 0, _num_vertexes);
	}
	else
	{

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cout << "5GL ERROR:    " << error << std::endl;
		}

		glDrawArrays(GL_TRIANGLES, 0, _num_vertexes);
	}

}

