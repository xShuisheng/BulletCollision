#pragma once

#include "camera.h"
#include "DefaultRenderObject.h"
#include "Light.h"
#include "Shader.h"
#include "GLUI.h"
#include "EventCallback.h"
#include <vector>
#include "CollisionDispatch/btCollisionWorld.h"

class DefaultRenderManager
{
private:
	DefaultRenderManager();

public:
	~DefaultRenderManager();

	static DefaultRenderManager* getInstance();

	bool addPointLight(const PointLight& light)
	{
		if (pointLights.size() < maxPointLightsNum)
		{
			pointLights.push_back(light);
			return true;
		}
		else
		{
			return false;
		}
		
	}

	bool setDirLight(const DirLight& light)
	{
		dirLight = light;
	}

	bool setSpotLight(const SpotLight& light)
	{
		spotLight = light;
	}

	bool setDefaultViewpoint(const glm::vec3& center, const glm::vec3& size);
	bool init();

	/**
	Prepare openGL buffers and attribute pointers.
 	*/
	bool prepare();

	bool render();

	void addRenderObject(DefaultRenderObject* object)
	{
		renderObjects.push_back(object);
	}

private:
	void _setDefaultLights();
	void _setShaderLights(Shader& shader);
public:
	GLUI window;

	btCollisionWorld * collisionWorld=0;

	Camera camera;

	EventCallback2 eventCallback;

	// render obects
	std::vector<DefaultRenderObject*> renderObjects;

	// lights
	static int maxPointLightsNum;
	std::vector<PointLight> pointLights;
	DirLight dirLight;
	SpotLight spotLight;

	// Projection model
	glm::mat4 projection;
	// View model
	glm::mat4 view;

	// material
	glm::vec3 specular;
	float shininess;

private:
	static DefaultRenderManager instance;
};