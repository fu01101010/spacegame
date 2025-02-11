#include "scene.h"
#include <iostream>

unsigned int scene::width = 0;
unsigned int scene::height = 0;


// constructor
scene::scene() {}
scene::scene(int glfwCVMajor, int glfwCVMinor, const char* title, unsigned int _width, unsigned int _height) {

	glViewport(0, 0, _width, _height);

	width = _width;
	height = _height;
}
