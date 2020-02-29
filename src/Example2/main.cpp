#include <iostream>
#include "glad/glad.h"

#include "GLUI.h"
#include "RenderManager.h"
#include "Cube.h"

#include "CollisionDispatch/btCollisionWorld.h"
#include "CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "CollisionDispatch/btCollisionDispatcher.h"
#include "BroadphaseCollision/btDbvtBroadphase.h"
#include "CollisionShapes/btBoxShape.h"
#include "CollisionShapes/btConvexPolyhedron.h"

#include <vector>
#include <random>
#include <time.h>

using namespace std;

// 简单demo，单个物体和场景中其它对象的碰撞检测
// 请用 w,a,s,d 键移动其中一个box去碰另一个box
void demo()
{
	/// define renderer
	DefaultRenderManager* prender = DefaultRenderManager::getInstance();
	prender->camera.setPosition(glm::vec3(0.0, 0.0, 4.0));


	///collision configuration contains default setup for memory, collision setup
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	///collision dispatcher will dispatch narrow phase detection algorithm according to different kinds of collision shapes.
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	/// broad phase collision detector
	btDbvtBroadphase* broadphase = new btDbvtBroadphase();

	/// collision world 
	btCollisionWorld* collisionWorld = new btCollisionWorld(dispatcher, broadphase, collisionConfiguration);
	prender->collisionWorld = collisionWorld;


	// first object: the floor --------------
	btBoxShape* floor_box = new btBoxShape(btVector3(0.5, 0.5, 0.5));	// 输入参数为box的半宽
	Cube* floor = new Cube();
	floor->setCollisionShape(floor_box);

	floor->setSize(1.0, 1.0, 1.0);
	SimpleMaterial& floor_material = floor->getMaterial();
	floor_material.diffuse = glm::vec3(0.6, 0.4, 0.0);
	floor_material.specular = glm::vec3(0.6, 0.4, 0.0);

	// transformation
	btTransform& floor_trans = floor->getWorldTransform();
	floor_trans.setOrigin(btVector3(-1.0, -0.0, -0.0));
	floor->setInitTransform(floor_trans);

	prender->addRenderObject(floor);
	collisionWorld->addCollisionObject(floor);


	// box -------------------------
	btBoxShape* box_shape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
	Cube* box = new Cube();
	box->setCollisionShape(box_shape);
	box->setSize(1.0, 1.0, 1.0);

	btTransform& box_trans = box->getWorldTransform();
	box_trans.setOrigin(btVector3(1.0, 0, -0.0));
	box->setInitTransform(box_trans);

	prender->addRenderObject(box);
	collisionWorld->addCollisionObject(box);

	prender->eventCallback.setCube(box);


	// collision call back
	// 碰撞检测成功后的回调类
	// 碰撞发生时，会调用 btCollisionWorld::ContactResultCallback 的 addSingleResult 函数
	// 请继承 btCollisionWorld::ContactResultCallback 并定义自己的addSingleResult
	MyColCallBack collisionCallBack;


	// main loop
	while (!prender->window.windowClose())
	{
		
		// 检测box和场景中其它物体的碰撞
		// 检测成功后，会调用
		collisionWorld->contactTest(box, collisionCallBack);

		prender->render();

	}
}


// 简单demo，场景中所有对象的相互碰撞检测
// 请用 w,a,s,d 键移动其中一个box去碰另一个box
void demo2()
{
	/// define renderer
	DefaultRenderManager* prender = DefaultRenderManager::getInstance();
	prender->camera.setPosition(glm::vec3(0.0, 0.0, 4.0));


	///collision configuration contains default setup for memory, collision setup
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	///collision dispatcher will dispatch narrow phase detection algorithm according to different kinds of collision shapes.
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	/// broad phase collision detector
	btDbvtBroadphase* broadphase = new btDbvtBroadphase();

	/// collision world 
	btCollisionWorld* collisionWorld = new btCollisionWorld(dispatcher, broadphase, collisionConfiguration);
	prender->collisionWorld = collisionWorld;


	// first object: the floor --------------
	btBoxShape* floor_box = new btBoxShape(btVector3(0.5, 0.5, 0.5));	// 输入参数为box的半宽
	Cube* floor = new Cube();
	floor->setCollisionShape(floor_box);

	floor->setSize(1.0, 1.0, 1.0);
	SimpleMaterial& floor_material = floor->getMaterial();
	floor_material.diffuse = glm::vec3(0.6, 0.4, 0.0);
	floor_material.specular = glm::vec3(0.6, 0.4, 0.0);

	// transformation
	btTransform& floor_trans = floor->getWorldTransform();
	floor_trans.setOrigin(btVector3(-1.0, -0.0, -0.0));
	floor->setInitTransform(floor_trans);

	prender->addRenderObject(floor);
	collisionWorld->addCollisionObject(floor);


	// box -------------------------
	btBoxShape* box_shape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
	Cube* box = new Cube();
	box->setCollisionShape(box_shape);
	box->setSize(1.0, 1.0, 1.0);

	btTransform& box_trans = box->getWorldTransform();
	box_trans.setOrigin(btVector3(1.0, 0, -0.0));
	box->setInitTransform(box_trans);

	prender->addRenderObject(box);
	collisionWorld->addCollisionObject(box);

	prender->eventCallback.setCube(box);


	// main loop
	while (!prender->window.windowClose())
	{
		// collision detection of all CollisionObjects
		collisionWorld->performDiscreteCollisionDetection();

		std::vector<Cube*> m_collisionObjects;

		// get total number of collision manifolds
		// a persistent manifold is a contact point cache
		int numManifolds = collisionWorld->getDispatcher()->getNumManifolds();

		// get all persistent manifolds
		for (int i = 0; i<numManifolds; i++)
		{
			// get the i-th manifold
			btPersistentManifold* contactManifold = collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);

			// contact objects
			Cube* obA = const_cast<Cube*>(dynamic_cast<const Cube*>(contactManifold->getBody0()));
			Cube* obB = const_cast<Cube*>(dynamic_cast<const Cube*>(contactManifold->getBody1()));

			// a persistent manifold may cache 0 - 4 contact points
			int numContacts = contactManifold->getNumContacts();
			float min_dis = 10000;
			for (int j = 0; j<numContacts; j++)
			{
				// get contact point
				btManifoldPoint& pt = contactManifold->getContactPoint(j);

				float dis = pt.getDistance();
				if (min_dis > dis)
				{
					min_dis = dis;
				}

				if (dis <= 0.01f)
				{
					m_collisionObjects.push_back(obA);
					m_collisionObjects.push_back(obB);
					btVector3 posA = pt.getPositionWorldOnA();
					btVector3 posB = pt.getPositionWorldOnB();
					std::cout << "Collision: " << std::endl;
					std::cout << "    Pos A:  " << posA.getX() << "  " << posA.getY() << "  " << posA.getZ() << std::endl;
					std::cout << "    Pos B:  " << posB.getX() << "  " << posB.getY() << "  " << posB.getZ() << std::endl;
					
				}
			}

			if (min_dis < 0.01f)
			{
				std::cout << std::endl;
				obA->backToInit();
				obB->backToInit();
			}

		}

		prender->render();

	}
}





// 大场景demo，场景中所有对象的相互碰撞检测
// 请用 w,a,s,d 键移动其中一个box去碰另一个box
void demo3()
{
	/// define renderer
	DefaultRenderManager* prender = DefaultRenderManager::getInstance();
	prender->camera.setPosition(glm::vec3(0.0, 0.0, 4.0));


	///collision configuration contains default setup for memory, collision setup
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	///collision dispatcher will dispatch narrow phase detection algorithm according to different kinds of collision shapes.
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	/// broad phase collision detector
	btDbvtBroadphase* broadphase = new btDbvtBroadphase();

	/// collision world 
	btCollisionWorld* collisionWorld = new btCollisionWorld(dispatcher, broadphase, collisionConfiguration);
	prender->collisionWorld = collisionWorld;


	// first object: the floor --------------
	btBoxShape* floor_box = new btBoxShape(btVector3(0.5, 0.5, 0.5));	// 输入参数为box的半宽
	Cube* floor = new Cube();
	floor->setCollisionShape(floor_box);

	floor->setSize(1.0, 1.0, 1.0);
	SimpleMaterial& floor_material = floor->getMaterial();
	floor_material.diffuse = glm::vec3(0.6, 0.4, 0.0);
	floor_material.specular = glm::vec3(0.6, 0.4, 0.0);

	// transformation
	btTransform& floor_trans = floor->getWorldTransform();
	floor_trans.setOrigin(btVector3(-1.0, -0.0, -0.0));
	floor->setInitTransform(floor_trans);

	prender->addRenderObject(floor);
	collisionWorld->addCollisionObject(floor);


	// box -------------------------
	btBoxShape* box_shape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
	Cube* box = new Cube();
	box->setCollisionShape(box_shape);
	box->setSize(1.0, 1.0, 1.0);

	btTransform& box_trans = box->getWorldTransform();
	box_trans.setOrigin(btVector3(1.0, 0, -0.0));
	box->setInitTransform(box_trans);

	prender->addRenderObject(box);
	collisionWorld->addCollisionObject(box);
	prender->eventCallback.setCube(box);


	std::default_random_engine random_eng(clock());

	// 碰撞检测的时间主要花在narrow phase的可能碰撞物体间的逐对检测，所以检测时间和他们分布的密度有和大关系
	float range = 50.0;						// box在空间中分布的范围
	int nbox = 1000;						// box的数量
	for (int i = 0; i < nbox; ++i)
	{
		// box -------------------------
		//btBoxShape* box_shape1 = new btBoxShape(btVector3(0.5, 0.5, 0.5));
		Cube* box1 = new Cube();
		box1->setCollisionShape(box_shape);		// collisionShape 是可以重复给多个相同shape的collisionObject用的。
		box1->setSize(1.0, 1.0, 1.0);
		
		// 设置随机位置
		btVector3 origin;
		origin[0] = range * (random_eng() / (float)random_eng.max()) - range / 2.0;
		origin[1] = range * (random_eng() / (float)random_eng.max()) - range / 2.0;
		origin[2] = - range * (random_eng() / (float)random_eng.max());


		btTransform& box_trans1 = box1->getWorldTransform();
		box_trans1.setOrigin(origin);
		box1->setInitTransform(box_trans1);

		prender->addRenderObject(box1);
		collisionWorld->addCollisionObject(box1);

		//prender->eventCallback.setCube(box1);
	}


	// main loop
	while (!prender->window.windowClose())
	{
		float t1 = clock() / (double)CLOCKS_PER_SEC;

		// 进行所有物体的碰撞检测
		collisionWorld->performDiscreteCollisionDetection();

		std::vector<Cube*> m_collisionObjects;

		// 碰撞manifold的数量
		int numManifolds = collisionWorld->getDispatcher()->getNumManifolds();

		for (int i = 0; i<numManifolds; i++)
		{
			btPersistentManifold* contactManifold = collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);
			Cube* obA = const_cast<Cube*>(dynamic_cast<const Cube*>(contactManifold->getBody0()));
			Cube* obB = const_cast<Cube*>(dynamic_cast<const Cube*>(contactManifold->getBody1()));

			int numContacts = contactManifold->getNumContacts();
			float min_dis = 10000;
			for (int j = 0; j<numContacts; j++)
			{
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				float dis = pt.getDistance();
				if (min_dis > dis)
				{
					min_dis = dis;
				}

				if (dis <= 0.01f)
				{
					m_collisionObjects.push_back(obA);
					m_collisionObjects.push_back(obB);
					btVector3 posA = pt.getPositionWorldOnA();
					btVector3 posB = pt.getPositionWorldOnB();
					//std::cout << "Collision: " << std::endl;
					//std::cout << "    Pos A:  " << posA.getX() << "  " << posA.getY() << "  " << posA.getZ() << std::endl;
					//std::cout << "    Pos B:  " << posB.getX() << "  " << posB.getY() << "  " << posB.getZ() << std::endl;
				}
			}

			if (min_dis < 0.01f)
			{
				//std::cout << std::endl;
				obA->backToInit();
				obB->backToInit();
			}

		}

		float t2 = clock() / (double)CLOCKS_PER_SEC;

		cout << "[*] Collisiong Detection Time of "<< nbox << " boxes :  " << t2 - t1 << endl;

		prender->render();

	}
}


int main()
{

	//demo();
	demo2();
	//demo3();
	//DefaultRenderManager* prender = DefaultRenderManager::getInstance();

	//Cube cube;
	//prender->addRenderObject(&cube);
	//

	//while (!prender->window.windowClose())
	//{
	//	std::cout << "1" << endl;
	//	prender->render();

	//	//glfwSwapBuffers((prender->window.pglwindow));
	//	//glfwPollEvents();
	//}

	system("pause");
	return 0;
}