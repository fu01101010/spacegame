#include "screen.h"

#include "keyboard.h"
#include "mouse.h"

unsigned screen::SCR_WIDTH = 800;
unsigned screen::SCR_HEIGHT = 600;

void screen::framebufferSizeCallback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);

	SCR_HEIGHT = height;
	SCR_WIDTH = width;
}

screen::screen() : window(nullptr) {}

bool screen::init() {
	
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GameDemo", nullptr, nullptr);
	if (!window) {

		// window not created
		return false;
	}

	glfwMakeContextCurrent(window);

	return true;
}

void screen::setParameters() {

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glfwSetFramebufferSizeCallback(window, screen::framebufferSizeCallback);

	glfwSetKeyCallback(window, keyboard::keyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse::cursorPositionCallback);
	glfwSetMouseButtonCallback(window, mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, mouse::mouseWheelCallback);

	glEnable(GL_DEPTH_TEST);
}

void screen::update() {

	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void screen::newFrame() {

	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool screen::shouldClose() {

	return glfwWindowShouldClose(window);
}

void screen::setShouldClose(bool shouldClose) {

	glfwSetWindowShouldClose(window, shouldClose);
}

