#include "Cube.h"

#include "CollisionShapes/btBoxShape.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Cube::Cube()
{
	float vertexes[] = {
		-0.5f, -0.5f, -0.5f,		// 0
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,		// 4
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,		// 8
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,			// 12
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,		// 16
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f, 

		0.5f,  0.5f, -0.5f,			// 20
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};

	float normals[]= {
		0.0f,  0.0f, -1.0f, 
		0.0f,  0.0f, -1.0f, 
		0.0f,  0.0f, -1.0f, 
		0.0f,  0.0f, -1.0f, 

		0.0f,  0.0f,  1.0f, 
		0.0f,  0.0f,  1.0f, 
		0.0f,  0.0f,  1.0f, 
		0.0f,  0.0f,  1.0f, 

		-1.0f,  0.0f,  0.0f, 
		-1.0f,  0.0f,  0.0f, 
		-1.0f,  0.0f,  0.0f,  
		-1.0f,  0.0f,  0.0f,  

		1.0f,  0.0f,  0.0f, 
		1.0f,  0.0f,  0.0f,  
		1.0f,  0.0f,  0.0f, 
		1.0f,  0.0f,  0.0f, 

		0.0f, -1.0f,  0.0f, 
		0.0f, -1.0f,  0.0f,  
		0.0f, -1.0f,  0.0f,  
		0.0f, -1.0f,  0.0f, 

		0.0f,  1.0f,  0.0f, 
		0.0f,  1.0f,  0.0f, 
		0.0f,  1.0f,  0.0f, 
		0.0f,  1.0f,  0.0f
	};

	float texcoord[]= {
		0.0f,  0.0f,
		1.0f,  0.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,

		0.0f,  0.0f,
		1.0f,  0.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,

		1.0f,  0.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  0.0f,

		1.0f,  0.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  0.0f,

		0.0f,  1.0f,
		1.0f,  1.0f,
		1.0f,  0.0f,
		0.0f,  0.0f,

		1.0f,  1.0f,
		1.0f,  0.0f,
		0.0f,  0.0f,
		0.0f,  1.0f
	};

	unsigned int indexes[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	_array_copy(vertexes, _vertexes, _num_vertexes*3);
	_array_copy(normals, _normals, _num_vertexes * 3);
	_array_copy(texcoord, _texcoord, _num_vertexes * 2);
	_array_copy(indexes, _indexes, _num_triangles * 3);


	//_material.diffuse.loadTexture("container2.png");
	//_material.specular = glm::vec3(0.1, 0.1, 0.1);
	_simple_material.shininess = 32.0;
	//loadTexture(_texture_path.c_str());

	setVertexes(_num_vertexes, _vertexes);
	setNormal(_normals);
	setTexcoord(_texcoord);
	setIndexes(_num_triangles, _indexes);

	bindAll();


	/// set Collision shape
	//btBoxShape* box = new btBoxShape(btVector3(1.0, 1.0, 1.0));
	//this->setCollisionShape(box);


}

void Cube::render(glm::mat4 transform)
{
	float mat[16];
	m_worldTransform.getOpenGLMatrix(&(mat[0]));

	_mat_model = glm::make_mat4(mat);
	_mat_model = glm::scale(_mat_model, glm::vec3(_sizex, _sizey, _sizez));

	//glm::mat4 mat_scale;
	DefaultRenderObject::render(transform);
}

