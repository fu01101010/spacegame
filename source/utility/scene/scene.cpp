#include "scene.h"
#include <iostream>

unsigned int scene::width = 0;
unsigned int scene::height = 0;


// constructor
scene::scene() {}
scene::scene(int glfwCVMajor, int glfwCVMinor, const char* title, unsigned int _width, unsigned int _height) : glfwCVMajor(glfwCVMajor), glfwCVMinor(glfwCVMinor), title(title), activeCamera(-1) {

	width = _width;
	height = _height;

	setWindowColor(0.7f, 0.7f, 0.7f, 1.0f);
}


// callbacks
	
// window resize
void scene::framebufferSizeCallback(GLFWwindow* window, int _width, int _height) {
	
	glViewport(0, 0, _width, _height);

	width = _width;
	height = _height;
}


// init method
bool scene::init() {
	
	glfwInit();

	// set version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwCVMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwCVMinor);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// init window
	window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (window == NULL) { // not created
	
		std::cout << "failed to create window" << std::endl;
		return false;
	}
	
	glfwMakeContextCurrent(window);

	// set GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		std::cout << "failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	// setup screen
	glViewport(0, 0, width, height);
	
	// callback funcs
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetKeyCallback(window, keyboard::keyCallback);
	glfwSetCursorPosCallback(window, mouse::cursorPositionCallback);
	glfwSetMouseButtonCallback(window, mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, mouse::mouseWheelCallback);

	// rendering params
	glEnable(GL_DEPTH_TEST);					// depth test duh
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// disable cursor
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		// set text func
	
	return true;
}


// process input method
void scene::processInput(double deltaTime) {

	if (activeCamera != -1 && activeCamera < cameras.size()) { // if active cam exists

		// update direction
		cameras[activeCamera]->updateCameraDirection(mouse::getDX(), mouse::getDY());

		// update zoom
		cameras[activeCamera]->updateCameraZoom(mouse::getScrollDY());

		// move camera
		if (keyboard::key(GLFW_KEY_E)) {

			cameras[activeCamera]->updateCameraPosition(cameraDirection::FORWARD, deltaTime);
		}
		
		if (keyboard::key(GLFW_KEY_D)) {

			cameras[activeCamera]->updateCameraPosition(cameraDirection::BACKWARD, deltaTime);
		}

		if (keyboard::key(GLFW_KEY_F)) {

			cameras[activeCamera]->updateCameraPosition(cameraDirection::RIGHT, deltaTime);
		}
		
		if (keyboard::key(GLFW_KEY_S)) {

			cameras[activeCamera]->updateCameraPosition(cameraDirection::LEFT, deltaTime);
		}
		
		if (keyboard::key(GLFW_KEY_SPACE)) {

			cameras[activeCamera]->updateCameraPosition(cameraDirection::UP, deltaTime);
		}
		
		if (keyboard::key(GLFW_KEY_Z)) {
		
			cameras[activeCamera]->updateCameraPosition(cameraDirection::DOWN, deltaTime);
		}

		view = cameras[activeCamera]->getViewMatrix();
		projection = glm::perspective(
			glm::radians(cameras[activeCamera]->getZoom()),	// fov
			(float)width / (float)height,			// aspect ratio
			0.1f, 100.0f					// near and far bounds
		);

		// set pos at end
		cameraPosition = cameras[activeCamera]->cameraPosition;

		// update spot lights
		for (unsigned int i = 0, nLights = spotLights.size(); i < nLights; ++i) {

			if ((activeSpotLights & (1 << i)) == (1 << i)) {
				
				spotLights[i]->position = cameras[activeCamera]->cameraPosition;
				spotLights[i]->direction = cameras[activeCamera]->cameraFront;
			}
		}	
	}
}


// update screen before each frame
void scene::update() {

	glClearColor(color[0], color[1], color[2], color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


// update screen after each frame
void scene::newFrame() {

	glfwSwapBuffers(window);
	glfwPollEvents();
}


// set vars + render
void scene::render(shader Shader) {
	
	Shader.activate();

	// set camera values
	Shader.setmat4("view", view);
	Shader.setmat4("projection", projection);
	Shader.set3flt("viewPos", cameraPosition);

	// lights

	// point lights
	unsigned int nLights = pointLights.size();
	Shader.set_int("nPointLights", nLights);

	unsigned int nActiveLights = 0;

	for (unsigned int i = 0; i < nLights; ++i) {
		
		if ((activePointLights & (1 << i)) == (1 << i)) {

			pointLights[i]->render(Shader, nActiveLights);
			nActiveLights++;
		}
	}

	// spot lights
	nLights = spotLights.size();
	Shader.set_int("nSpotLights", nLights);
	nActiveLights = 0;

 	for (unsigned int i = 0; i < nLights; ++i) {

		if ((activeSpotLights & (1 << i)) == (1 << i)) {
			
			spotLights[i]->render(Shader, nActiveLights);
			nActiveLights++;
		}
	}

	// dir lights
	nLights = directLights.size();
	Shader.set_int("nDirectLights", nLights);
	nActiveLights = 0;

 	for (unsigned int i = 0; i < nLights; ++i) {

		if ((activeDirectLights & (1 << i)) == (1 << i)) {
			
			directLights[i]->render(Shader, nActiveLights);
			nActiveLights++;
		}
	}
}


void scene::cleanUp() {

	glfwTerminate();
}

// accesor funcs
bool scene::shouldClose() {

	return glfwWindowShouldClose(window);
}


// modifier funcs
void scene::setShouldClose(bool shouldClose) {
	
	glfwSetWindowShouldClose(window, shouldClose);
}

void scene::setWindowColor(float r, float g, float b, float a) {
	
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}
