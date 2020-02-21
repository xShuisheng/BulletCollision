#pragma once
#include "GLFW\glfw3.h"
#include "camera.h"
#include "CollisionDispatch/btCollisionWorld.h"
#include "Cube.h"


class EventCallback
{
public:
	EventCallback(int lastx=0, int lasty=0)
	{
		_lastX = lastx;
		_lastY = lasty;
	}

	void setCamera(Camera* pcamera)
	{
		_pcurCamera = pcamera;
	}

	void setLastPos(int lastx, int lasty)
	{
		_lastX = lastx;
		_lastY = lasty;
	}

	void mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		if (_firstMouse)
		{
			_lastX = xpos;
			_lastY = ypos;
			_firstMouse = false;
		}

		float xoffset = xpos - _lastX;
		float yoffset = _lastY - ypos; // reversed since y-coordinates go from bottom to top

		_lastX = xpos;
		_lastY = ypos;

		_pcurCamera->ProcessMouseMovement(xoffset, yoffset);
	}

	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		_pcurCamera->ProcessMouseScroll(yoffset);
	}

	void processInput(GLFWwindow *window)
	{
		float currentFrame = glfwGetTime();
		_deltaTime = currentFrame - _lastFrame;
		_lastFrame = currentFrame;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			_pcurCamera->ProcessKeyboard(FORWARD, _deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			_pcurCamera->ProcessKeyboard(BACKWARD, _deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			_pcurCamera->ProcessKeyboard(LEFT, _deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			_pcurCamera->ProcessKeyboard(RIGHT, _deltaTime);
	}

	void setCollisionWorld(btCollisionWorld* world) { this->_collisionWorld = world; }

private:
	btCollisionWorld* _collisionWorld = 0;

	bool _firstMouse = true;
	int _lastX = 0;
	int _lastY = 0;

	float _lastFrame = 0;
	float _deltaTime = 0;

	Camera* _pcurCamera = 0;
};

class EventCallback2
{
public:
	EventCallback2(int lastx = 0, int lasty = 0)
	{

	}

	void setCamera(Camera* pcamera)
	{
		_pcurCamera = pcamera;
	}

	void setLastPos(int lastx, int lasty)
	{
		_lastX = lastx;
		_lastY = lasty;
	}

	void mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		if (_firstMouse)
		{
			_lastX = xpos;
			_lastY = ypos;
			_firstMouse = false;
		}

		float xoffset = xpos - _lastX;
		float yoffset = _lastY - ypos; // reversed since y-coordinates go from bottom to top

		_lastX = xpos;
		_lastY = ypos;

		_pcurCamera->ProcessMouseMovement(xoffset, yoffset);
	}

	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		_pcurCamera->ProcessMouseScroll(yoffset);
	}

	void processInput(GLFWwindow *window)
	{
		float currentFrame = glfwGetTime();
		_deltaTime = currentFrame - _lastFrame;
		_lastFrame = currentFrame;

		//btVector3 velocity(0.5)
		float velocity = 0.5;
		float d = velocity*_deltaTime;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			//_pcurCamera->ProcessKeyboard(FORWARD, _deltaTime);
			_cube->move(btVector3(0.0, 1.0, 0.0)*d);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			_cube->move(btVector3(0.0, -1.0, 0.0)*d);
		}
			//_pcurCamera->ProcessKeyboard(BACKWARD, _deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			_cube->move(btVector3(-1.0, 0.0, 0.0)*d);
		}
			//_pcurCamera->ProcessKeyboard(LEFT, _deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			_cube->move(btVector3(1.0, 0.0, 0.0)*d);
		}
			//_pcurCamera->ProcessKeyboard(RIGHT, _deltaTime);
	}

	void setCollisionWorld(btCollisionWorld* world) { this->_collisionWorld = world; }
	void setCube(Cube* cube) { this->_cube = cube; }
private:
	btCollisionWorld* _collisionWorld = 0;
	Cube* _cube = 0;

	bool _firstMouse = true;
	int _lastX = 0;
	int _lastY = 0;

	float _lastFrame = 0;
	float _deltaTime = 0;

	Camera* _pcurCamera = 0;
};