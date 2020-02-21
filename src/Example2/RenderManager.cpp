#include "RenderManager.h"
#include "glm\glm.hpp"

int DefaultRenderManager::maxPointLightsNum = 4;
DefaultRenderManager DefaultRenderManager::instance;

void defaultrendermanager_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	DefaultRenderManager::getInstance()->eventCallback.mouseCallback(window, xpos, ypos);
}

void defaultrendermanager_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	DefaultRenderManager::getInstance()->eventCallback.scrollCallback(window, xoffset, yoffset);
}

DefaultRenderManager::DefaultRenderManager():window(GLUI())
{
	
	window.pcursorCallback = defaultrendermanager_mouse_callback;
	window.pscrollCallback = defaultrendermanager_scroll_callback;
	window.init();
	


	eventCallback.setLastPos(window.windowWidth / 2, window.windowHeight / 2);
	eventCallback.setCamera(&camera);

	// init default lights
	_setDefaultLights();

	// init materials
	specular = glm::vec3(0.2, 0.2, 0.2);
	shininess = 32.0;


	camera.Position = glm::vec3(0, 0, 2.0);
}

DefaultRenderManager::~DefaultRenderManager()
{
	for (int i = 0; i < renderObjects.size(); ++i)
	{
		delete renderObjects[i];
	}
}

DefaultRenderManager * DefaultRenderManager::getInstance()
{
	return &instance;
}

bool DefaultRenderManager::setDefaultViewpoint(const glm::vec3 & center, const glm::vec3 & size)
{
	float dis = size.x > size.y ? size.x : size.y;
	dis = dis > size.z ? dis : size.z;
	dis *= 1.5;

	glm::vec3 pos(0.0, 0.0, 0.0);
	pos.y = dis* 0.707;
	pos.z = pos.y;
	pos += center;

	camera.Position = pos;
	camera.Pitch = -45.0;
	camera.updateCameraVectors();

	return true;
}

bool DefaultRenderManager::init()
{
	//window.init();
	return true;
}

bool DefaultRenderManager::prepare()
{
	for (int i = 0; i < renderObjects.size(); ++i)
	{
		//renderObjects[i].bindAll();

		int error = -1;
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cout << "here ERROR:    " << error << std::endl;
		}
	}
	return true;
}

bool DefaultRenderManager::render()
{
	eventCallback.processInput(window.pglwindow);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	projection = glm::perspective(glm::radians(camera.Zoom), (float)window.windowWidth / (float)window.windowHeight, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	glm::mat4 trans = projection*view;

	_setDefaultLights();
	for (int i = 0; i < renderObjects.size(); ++i)
	{
		Shader* pshader = renderObjects[i]->pshader;

		pshader->use();
		pshader->setVec3("viewPos", camera.Position);
		_setShaderLights(*pshader);

		renderObjects[i]->render(trans);
	}

	glfwSwapBuffers(window.pglwindow);
	glfwPollEvents();

	return true;
}

void DefaultRenderManager::_setDefaultLights()
{
	// directional light
	dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	dirLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	dirLight.diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	dirLight.specular = glm::vec3(0.3f, 0.3f, 0.3f);

	pointLights.resize(4);
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	// point light 1
	pointLights[0].position = glm::vec3(pointLightPositions[0]);
	pointLights[0].ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	pointLights[0].diffuse = glm::vec3(0.f, 0.f, 0.f);
	pointLights[0].specular = glm::vec3(0.0f, 0.0f, 0.0f);
	pointLights[0].constant = 1.0f;
	pointLights[0].linear = 0.09;
	pointLights[0].quadratic = 0.032;
	// point light 2
	pointLights[1].position = pointLightPositions[1];
	pointLights[1].ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	pointLights[1].diffuse = glm::vec3(0.f, 0.f, 0.f);
	pointLights[1].specular = glm::vec3(0.0f, 0.0f, 0.0f);
	pointLights[1].constant = 1.0f;
	pointLights[1].linear = 0.09;
	pointLights[1].quadratic = 0.032;
	// point light 3
	pointLights[2].position = pointLightPositions[2];
	pointLights[2].ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	pointLights[2].diffuse = glm::vec3(0.f, 0.f, 0.f);
	pointLights[2].specular = glm::vec3(0.0f, 0.0f, 0.0f);
	pointLights[2].constant = 1.0f;
	pointLights[2].linear = 0.09;
	pointLights[2].quadratic = 0.032;
	// point light 4
	pointLights[3].position = pointLightPositions[3];
	pointLights[3].ambient = glm::vec3(0.0f, 0.00f, 0.0f);
	pointLights[3].diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	pointLights[3].specular = glm::vec3(0.0f, 0.0f, 0.0f);
	pointLights[3].constant = 1.0f;
	pointLights[3].linear = 0.09;
	pointLights[3].quadratic = 0.032;

	// spotLight
	spotLight.position = camera.Position;
	spotLight.direction = camera.Front;
	spotLight.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	spotLight.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	spotLight.specular = glm::vec3(0.0f, 0.0f, 0.0f);
	spotLight.constant = 1.0f;
	spotLight.linear = 0.09;
	spotLight.quadratic = 0.032;
	spotLight.cutOff = glm::cos(glm::radians(12.5f));
	spotLight.outerCutOff = glm::cos(glm::radians(15.0f));
}

void DefaultRenderManager::_setShaderLights(Shader & shader)
{
	int error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "seting shader lights ERROR:    " << error << std::endl;
	}

	// directional light
	shader.setVec3("dirLight.direction", dirLight.direction);
	shader.setVec3("dirLight.ambient", dirLight.ambient);
	shader.setVec3("dirLight.diffuse", dirLight.diffuse);
	shader.setVec3("dirLight.specular", dirLight.specular);

	error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "seting shader lights ERROR:    " << error << std::endl;
	}

	// point light 1
	shader.setVec3("pointLights[0].position", pointLights[0].position);
	shader.setVec3("pointLights[0].ambient", pointLights[0].ambient);
	shader.setVec3("pointLights[0].diffuse", pointLights[0].diffuse);
	shader.setVec3("pointLights[0].specular", pointLights[0].specular);
	shader.setFloat("pointLights[0].constant", pointLights[0].constant);
	shader.setFloat("pointLights[0].linear", pointLights[0].linear);
	shader.setFloat("pointLights[0].quadratic", pointLights[0].quadratic);

	error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "seting shader lights ERROR:    " << error << std::endl;
	}
	// point light 2
	shader.setVec3("pointLights[1].position", pointLights[1].position);
	shader.setVec3("pointLights[1].ambient", pointLights[1].ambient);
	shader.setVec3("pointLights[1].diffuse", pointLights[1].diffuse);
	shader.setVec3("pointLights[1].specular", pointLights[1].specular);
	shader.setFloat("pointLights[1].constant", pointLights[1].constant);
	shader.setFloat("pointLights[1].linear", pointLights[1].linear);
	shader.setFloat("pointLights[1].quadratic", pointLights[1].quadratic);

	error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "seting shader lights ERROR:    " << error << std::endl;
	}
	// point light 3
	shader.setVec3("pointLights[2].position", pointLights[2].position);
	shader.setVec3("pointLights[2].ambient", pointLights[2].ambient);
	shader.setVec3("pointLights[2].diffuse", pointLights[2].diffuse);
	shader.setVec3("pointLights[2].specular", pointLights[2].specular);
	shader.setFloat("pointLights[2].constant", pointLights[2].constant);
	shader.setFloat("pointLights[2].linear", pointLights[2].linear);
	shader.setFloat("pointLights[2].quadratic", pointLights[2].quadratic);

	error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "seting shader lights ERROR:    " << error << std::endl;
	}
	// point light 4
	shader.setVec3("pointLights[3].position", pointLights[3].position);
	shader.setVec3("pointLights[3].ambient", pointLights[3].ambient);
	shader.setVec3("pointLights[3].diffuse", pointLights[3].diffuse);
	shader.setVec3("pointLights[3].specular", pointLights[3].specular);
	shader.setFloat("pointLights[3].constant", pointLights[3].constant);
	shader.setFloat("pointLights[3].linear", pointLights[3].linear);
	shader.setFloat("pointLights[3].quadratic", pointLights[3].quadratic);

	error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "seting shader lights ERROR:    " << error << std::endl;
	}
	// spotLight
	shader.setVec3("spotLight.position", spotLight.position);
	shader.setVec3("spotLight.direction", spotLight.direction);
	shader.setVec3("spotLight.ambient", spotLight.ambient);
	shader.setVec3("spotLight.diffuse", spotLight.diffuse);
	shader.setVec3("spotLight.specular", spotLight.specular);
	shader.setFloat("spotLight.constant", spotLight.constant);
	shader.setFloat("spotLight.linear", spotLight.linear);
	shader.setFloat("spotLight.quadratic", spotLight.quadratic);
	shader.setFloat("spotLight.cutOff", spotLight.cutOff);
	shader.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);

	error = -1;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "seting shader lights ERROR:    " << error << std::endl;
	}
}


