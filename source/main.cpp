#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

#include "../source/graphics/shader.h"
#include "../source/graphics/textures/texture.h"

#include "../source/graphics/utility/model.h"
#include "../source/graphics/light.h"

#include "../source/graphics/models/vcube.hpp"
#include "../source/graphics/models/ucube.hpp"
#include "../source/graphics/models/vlightsource.hpp"

#include "../source/io/mouse.h"
#include "../source/io/keyboard.h"
#include "../source/io/screen.h"
#include "../source/io/camera.h"

#include "../source/utility/terrain/terrain.h"

// funcs here
void processInput(double deltaTime);


// io here
screen Screen;

double dx, dy;
double scrollDX, scrollDY;

double deltaTime = 0.0f; // time inbetween frames
double lastFrame = 0.0f; // time of last frame


// temporary 
camera camera::defaultCamera(glm::vec3(0.0f, 1.0f, 0.0f));

bool wireframeFlag = false;

terrain UTerrain = terrain(material::black_plastic, glm::vec3(0.0f), "/Users/ulysses/Desktop/source/projects/game/source/assets/heightMaps/image.jpg", 0.01f);



// main
int main() {

	// glfw: initialize and configure

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	std::cout << "Hello, endless space!" << std::endl;

	// glfw window creation
	if (!Screen.init()) {
		
		std::cout << "failed to open window" << std::endl;
		glfwTerminate();

		return -1;
	}

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Screen.setParameters();

	// shaders
	shader mShader("/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/ucore.vs", "/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/ucore.fs");
	shader uShader("/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/mcore.vs", "/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/mcore.fs");

	m_model garbage(glm::vec3(0.0f, 0.0f, -6.0f), glm::vec3(1.0f), false);
	garbage.loadModel("/Users/ulysses/Desktop/source/projects/game/source/assets/models/garbage/scene.gltf");

	m_spotLight uSpotLight = {
		camera::defaultCamera.cameraPosition, camera::defaultCamera.cameraFront,
		glm::cos(glm::radians(20.5f)), glm::cos(glm::radians(25.5f)),
		1.0f, 0.0f, 0.0f,
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f)
	};

	UTerrain.init();

	// render loop
	while (!Screen.shouldClose()) {
		
		// calculate deltaTime
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// process input
		processInput(deltaTime);

		// render
		Screen.update();
		
		uShader.activate();

		uShader.set3flt("viewPos", camera::defaultCamera.cameraPosition);

		uSpotLight.position = camera::defaultCamera.cameraPosition;
		uSpotLight.direction = camera::defaultCamera.cameraFront;
		uSpotLight.render(uShader);				

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = camera::defaultCamera.getViewMatrix();
		projection = glm::perspective(
			glm::radians(camera::defaultCamera.zoom), 
			(float)screen::SCR_WIDTH / (float)screen::SCR_HEIGHT, 0.1f, 100.0f
		);

		uShader.setmat4("view", view);
		uShader.setmat4("projection", projection);

		//UTerrain.render(uShader);
		garbage.render(uShader);	

		// send new frame to window
		Screen.newFrame();
		glfwPollEvents();
	}
	
	UTerrain.cleanUp();
	garbage.cleanUp();

	// glfw: terminate, clearing all previously allocated GLFW resources.

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly

void processInput(double deltaTime) {
	
	if (keyboard::key(GLFW_KEY_ESCAPE)) {
		
		Screen.setShouldClose(true);
	}

	// move camera
	if (keyboard::key(GLFW_KEY_E)) {

		camera::defaultCamera.updateCameraPosition(cameraDirection::FORWARD, deltaTime);
	}
	if (keyboard::key(GLFW_KEY_D)) {

		camera::defaultCamera.updateCameraPosition(cameraDirection::BACKWARD, deltaTime);
	}
	if (keyboard::key(GLFW_KEY_F)) {

		camera::defaultCamera.updateCameraPosition(cameraDirection::RIGHT, deltaTime);
	}
	if (keyboard::key(GLFW_KEY_S)) {

		camera::defaultCamera.updateCameraPosition(cameraDirection::LEFT, deltaTime);
	}
	if (keyboard::key(GLFW_KEY_SPACE)) {

		camera::defaultCamera.updateCameraPosition(cameraDirection::UP, deltaTime);
	}
	if (keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
		
		camera::defaultCamera.updateCameraPosition(cameraDirection::DOWN, deltaTime);
	}

	if (keyboard::keyWentDn(GLFW_KEY_R)) {
		
		wireframeFlag = !wireframeFlag;

		if (wireframeFlag) {
			
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	dx = mouse::getDX();
	dy = mouse::getDY();

	if (dx != 0 || dy != 0) {

		camera::defaultCamera.updateCameraDirection(dx, dy);
	}

	scrollDY = mouse::getScrollDY();

	if (scrollDY != 0) {

		camera::defaultCamera.updateCameraZoom(scrollDY);
	}
}
