#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLUI
{
public:
	GLUI();
	GLUI(int width, int height);

	void setSize(int width, int height)
	{
		windowWidth = width;
		windowHeight = height;
	}

	bool windowClose();

	bool init();

	void resizeCallback(GLFWwindow* window, int width, int height);
	//virtual void cursorCallback(GLFWwindow* window, double xpos, double ypos);
	//virtual void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:
	int windowWidth = 800;
	int windowHeight = 600;

	GLFWwindow* pglwindow = 0;

	GLFWframebuffersizefun presizeCallback=0;
	GLFWcursorposfun pcursorCallback = 0;
	GLFWscrollfun pscrollCallback = 0;
};