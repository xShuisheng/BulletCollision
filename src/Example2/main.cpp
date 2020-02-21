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

using namespace std;

// ��demo����������ͳ����������������ײ���
// ���� w,a,s,d ���ƶ�����һ��boxȥ����һ��box
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
	btBoxShape* floor_box = new btBoxShape(btVector3(0.5, 0.5, 0.5));	// �������Ϊbox�İ��
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
	// ��ײ���ɹ���Ļص���
	// ��ײ����ʱ������� btCollisionWorld::ContactResultCallback �� addSingleResult ����
	// ��̳� btCollisionWorld::ContactResultCallback �������Լ���addSingleResult
	MyColCallBack collisionCallBack;


	// main loop
	while (!prender->window.windowClose())
	{
		
		// ���box�ͳ����������������ײ
		// ���ɹ��󣬻����
		collisionWorld->contactTest(box, collisionCallBack);

		prender->render();

	}
}


// ��demo�����������ж�����໥��ײ���
// ���� w,a,s,d ���ƶ�����һ��boxȥ����һ��box
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
	btBoxShape* floor_box = new btBoxShape(btVector3(0.5, 0.5, 0.5));	// �������Ϊbox�İ��
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
		// ���������������ײ���
		collisionWorld->performDiscreteCollisionDetection();

		std::vector<Cube*> m_collisionObjects;

		// ��ײmanifold������
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

int main()
{

	//demo();
	demo2();

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