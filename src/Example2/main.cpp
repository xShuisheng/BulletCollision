#include <iostream>
#include "glad/glad.h"

#include "GLUI.h"
#include "RenderManager.h"
#include "Cubic.h"

using namespace std;

int main()
{

	DefaultRenderManager* prender = DefaultRenderManager::getInstance();

	Cubic cube;
	prender->addRenderObject(&cube);
	

	while (!prender->window.windowClose())
	{
		std::cout << "1" << endl;
		prender->render();

		//glfwSwapBuffers((prender->window.pglwindow));
		//glfwPollEvents();
	}

	system("pause");
	return 0;
}