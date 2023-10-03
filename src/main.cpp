#include <iostream>
#include "Engine/Engine.h"

int main()
{
	Engine engine;
	engine.init();

	std::cout << "Hello, CMake" << std::endl;

	return 0;
}