#pragma once

#include "DefaultRenderObject.h"
#include <string>
#include "glm\glm.hpp"
#include "Material.h"
#include "CollisionDispatch/btCollisionObject.h"
#include <iostream>
#include "CollisionDispatch/btCollisionWorld.h"

class Cube:public DefaultRenderObject, public btCollisionObject
{
public:
	Cube();

	void setSize(float sx, float sy, float sz)
	{
		this->_sizex = sx;
		this->_sizey = sy;
		this->_sizez = sz;
	}

	void render(glm::mat4 transform);
	
	void move(const btVector3 & ds)
	{
		this->m_worldTransform.setOrigin(m_worldTransform.getOrigin() + ds);
	}


	void setInitTransform(const btTransform& trans) { _initTransform = trans; }

	void backToInit()
	{
		m_worldTransform = _initTransform;
	}
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

	float _sizex = 1.0;
	float _sizey = 1.0;
	float _sizez = 1.0;

	btTransform _initTransform;
};

// 碰撞检测回调
// 一个简单的示例，一旦发生碰撞，两个cube回到原位
struct MyColCallBack : btCollisionWorld::ContactResultCallback
{
public:
	btScalar addSingleResult(
		btManifoldPoint & cp,
		const btCollisionObjectWrapper * colObj0Wrap,
		int partId0,
		int index0,
		const btCollisionObjectWrapper * colObj1Wrap,
		int partId1,
		int index1)
	{
		btVector3 posA = cp.getPositionWorldOnA();
		btVector3 posB = cp.getPositionWorldOnB();

		// 
		std::cout << "Collision: " << std::endl;
		std::cout << "    Pos A:  " << posA.getX() << "  " << posA.getY() << "  " << posA.getZ() << std::endl;
		std::cout << "    Pos B:  " << posB.getX() << "  " << posB.getY() << "  " << posB.getZ() << std::endl;

		Cube* cube1 = const_cast<Cube*>(dynamic_cast<const Cube *>(colObj0Wrap->getCollisionObject()));
		cube1->backToInit();
		//cube1->backToInit();

		Cube* cube2 = const_cast<Cube*>(dynamic_cast<const Cube *>(colObj1Wrap->getCollisionObject()));
		cube2->backToInit();

		return btScalar(0.f);
	};
};