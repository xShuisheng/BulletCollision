#include "GLUI.h"


#include <iostream>

void GLUI_ResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLUI::GLUI()
{
	presizeCallback = GLUI_ResizeCallback;
	//presizeCallback = GLUI::resizeCallback;
}

GLUI::GLUI(int width, int height)
{
	presizeCallback = GLUI_ResizeCallback;

	windowWidth = width;
	windowHeight = height;
}

bool GLUI::windowClose()
{
	return glfwWindowShouldClose(pglwindow);
}

bool GLUI::init()
{
	// glfw: initialize and configure
	// ------------------------------
	//glewExperimental = true;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "GLUI", NULL, NULL);
	pglwindow = window;

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, presizeCallback);
	if (pcursorCallback)
	{
		glfwSetCursorPosCallback(window, pcursorCallback);
	}
	if (pscrollCallback)
	{
		glfwSetScrollCallback(window, pscrollCallback);
	}
	

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	return true;
}

void GLUI::resizeCallback(GLFWwindow * window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}



