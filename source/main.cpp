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

void processInput(double deltaTime);

screen Screen;
camera camera::defaultCamera(glm::vec3(0.0f, 1.0f, 0.0f));

double dx, dy;
double scrollDX, scrollDY;

bool wireframeFlag = false;

double deltaTime = 0.0f; // time inbetween frames
double lastFrame = 0.0f; // time of last frame

ucube UCube = ucube(material::brass, glm::vec3(0.0f), glm::vec3(1.4f));
vcube VCube = vcube(material::white_plastic, glm::vec3(0.0f), glm::vec3(1.4f));

terrain UTerrain = terrain(material::black_plastic, glm::vec3(0.0f), "/Users/ulysses/Desktop/source/projects/game/source/assets/heightMaps/image.jpg", 0.01f);

int main()
{

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
	shader uShader("/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/ucore.vs", "/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/ucore.fs");
	shader vShader("/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/mcore.vs", "/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/mcore.fs");
	shader lightSourceShader("/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/vcore.vs", "/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/lightsource.fs");

	m_model trollModel(glm::vec3(0.0f, 0.0f, -6.0f), glm::vec3(0.05f), false);
	//m_model eagle(glm::vec3(0.0f, 0.0f, -6.0f), glm::vec3(0.2f), false);
	//dModel superHorny(glm::vec3(0.0f, 0.0f, -6.0f), glm::vec3(0.05f), false);
	trollModel.loadModel("/Users/ulysses/Desktop/source/projects/game/source/assets/models/lotr_troll/scene.gltf");
	//eagle.loadModel("/Users/ulysses/Desktop/source/projects/game/source/assets/models/eagle/scene.gltf");
	//superHorny.loadModel("/Users/ulysses/Desktop/source/projects/game/source/assets/models/super_horny/scene.gltf");

	// lights
	m_directLight DirectLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) };

	spotLight uSpotLight = {
		camera::defaultCamera.cameraPosition, camera::defaultCamera.cameraFront,
		glm::cos(glm::radians(20.5f)), glm::cos(glm::radians(25.5f)),
		1.0f, 0.0f, 0.0f,
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f)
	};

	m_spotLight SpotLight = {
		camera::defaultCamera.cameraPosition, camera::defaultCamera.cameraFront,
		glm::cos(glm::radians(20.5f)), glm::cos(glm::radians(25.5f)),
		1.0f, 0.0f, 0.0f,
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f)
	};
	// 	1.0f, 0.07f, 0.032f,
	
	VCube.init();
	UCube.init();
	UTerrain.init();

	//vLightSource lamp = vLightSource(glm::vec3(1.0f), glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.25f));
	//lamp.init();

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
		
		//vShader.activate();

		//vShader.set3flt("viewPos", camera::defaultCamera.cameraPosition);

		//DirectLight.render(vShader);
		//vShader.set_int("nPointLights", 1);
		//lamp.PointLight.render(vShader, 0);

		//SpotLight.position = camera::defaultCamera.cameraPosition;
		//SpotLight.direction = camera::defaultCamera.cameraFront;
		//SpotLight.render(vShader);

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

		//vShader.setmat4("view", view);
		//vShader.setmat4("projection", projection);

		uShader.setmat4("view", view);
		uShader.setmat4("projection", projection);

		//std::cout << camera::defaultCamera.cameraPosition.x << ' ' << camera::defaultCamera.cameraPosition.y << ' ' << camera::defaultCamera.cameraPosition.z << std::endl;


		//trollModel.render(vShader);
		//eagle.render(vShader);
		//superHorny.render(vShader);
		//VCube.render(vShader);
		//UCube.render(uShader);
		UTerrain.render(uShader);

		lightSourceShader.activate();
		lightSourceShader.setmat4("view", view);
		lightSourceShader.setmat4("projection", projection);
	
		//lamp.render(lightSourceShader);

		// send new frame to window
		Screen.newFrame();
		glfwPollEvents();
	}
	
	UTerrain.cleanUp();

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
