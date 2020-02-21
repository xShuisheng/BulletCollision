#pragma once
#include "GLFW\glfw3.h"
#include "camera.h"


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

private:
	bool _firstMouse = true;
	int _lastX = 0;
	int _lastY = 0;

	float _lastFrame = 0;
	float _deltaTime = 0;

	Camera* _pcurCamera = 0;
};