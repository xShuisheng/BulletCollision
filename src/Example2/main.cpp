#include <iostream>
#include "glad/glad.h"

#include "GLUI.h"

using namespace std;

int main()
{

	GLUI ui;

	ui.init();

	while (!ui.windowClose())
	{
		std::cout << "1" << endl;
	}

	system("pause");
	return 0;
}