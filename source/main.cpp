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

#include "../source/graphics/shader.h"
#include "../source/graphics/textures/vTexture.h"
#include "../source/graphics/textures/dTexture.h"
#include "../source/graphics/models/vModel.h"
#include "../source/graphics/models/dModel.h"
#include "../source/graphics/light.h"

#include "../source/graphics/models/vCube.hpp"
#include "../source/graphics/models/dCube.hpp"
#include "../source/graphics/models/vLightsource.hpp"

#include "../source/io/mouse.h"
#include "../source/io/keyboard.h"
#include "../source/io/screen.h"
#include "../source/io/camera.h"

void processInput(double deltaTime);

screen Screen;

camera camera::defaultCamera(glm::vec3(0.0f, 0.0f, 3.0f));

double deltaTime = 0.0f; // time inbetween frames
double lastFrame = 0.0f; // time of last frame

std::string loadShaderSrc(const char* filename);

int main()
{
	int success;
	char infoLog[512];

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
	shader Shader("/Users/ulysses/Desktop/source/projects/gaussianNoiseVisual/source/shaders/core.vs", "/Users/ulysses/Desktop/source/projects/gaussianNoiseVisual/source/shaders/core.fs");
	shader lightSourceShader("/Users/ulysses/Desktop/source/projects/gaussianNoiseVisual/source/shaders/core.vs", "/Users/ulysses/Desktop/source/projects/gaussianNoiseVisual/source/shaders/lightsource.fs");

	// lights
	directLight DirectLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) };

	spotLight SpotLight = {
		camera::defaultCamera.cameraPosition, camera::defaultCamera.cameraFront,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f)),
		1.0f, 0.07f, 0.032f,
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f)
	};

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
		
		Shader.activate();

		Shader.set3flt("viewPos", camera::defaultCamera.cameraPosition);

		DirectLight.render(Shader);

		SpotLight.position = camera::defaultCamera.cameraPosition;
		SpotLight.direction = camera::defaultCamera.cameraFront;
		SpotLight.render(Shader, 0);		

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = camera::defaultCamera.getViewMatrix();
		projection = glm::perspective(
			glm::radians(camera::defaultCamera.zoom), 
			(float)screen::SCR_WIDTH / (float)screen::SCR_HEIGHT, 0.1f, 100.0f
		);

		Shader.setmat4("view", view);
		Shader.setmat4("projection", projection);

		lightSourceShader.activate();
		lightSourceShader.setmat4("view", view);
		lightSourceShader.setmat4("projection", projection);

		// send new frame to window
		Screen.newFrame();
		glfwPollEvents();
	}

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
}
