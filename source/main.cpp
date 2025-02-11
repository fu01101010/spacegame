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

#include <ft2build.h>
#include <freetype/freetype.h>

#include "../source/graphics/shader.h"
#include "../source/graphics/textures/texture.h"
#include "../source/graphics/text/text.h"

#include "../source/graphics/utility/model.h"
#include "../source/graphics/utility/weighedModel.h"
#include "../source/graphics/utility/staticModel.h"
#include "../source/graphics/light.h"

#include "../source/graphics/models/vcube.hpp"
#include "../source/graphics/models/ucube.hpp"
#include "../source/graphics/models/vlightsource.hpp"

#include "../source/io/mouse.h"
#include "../source/io/keyboard.h"
#include "../source/io/screen.h"
#include "../source/io/camera.h"

#include "../source/utility/terrain/terrain.h"
#include "../source/utility/math/mat.h"


// funcs here
void processInput(double deltaTime);


// io here
screen Screen;

double dx, dy;
double scrollDX, scrollDY;

double deltaTime = 0.0f; // time inbetween frames
double lastFrame = 0.0f; // time of last frame
double currentTime = 0.0f;


// temporary 
camera camera::defaultCamera(glm::vec3(0.0f, 1.0f, 0.0f));

bool wireframeFlag = false;

terrain UTerrain = terrain(material::black_plastic, glm::vec3(0.0f), "/Users/ulysses/Desktop/source/projects/game/source/assets/heightMaps/image.jpg", 0.01f);

int currentBone = 6;

int currentAnim = 0;

weighedModel coolmanny(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), false);

bool blinn = false;
bool gammaCorrection = false;

FT_Library ft;
ftText testText = ftText(32);

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
		
		std::cout << "failed to initialize GLAD" << std::endl;
		return -1;
	}

	Screen.setParameters();

	// init freetypelib
	if (FT_Init_FreeType(&ft)) {
		
		std::cout << "failed to initialize freetype lib" << std::endl;
		return -1;
	}

	if (!testText.load(ft, "/Users/ulysses/Desktop/source/projects/game/source/assets/fonts/mac/Monaco.ttf")) {
		
		std::cout << "failed to load font" << std::endl;
		return -1;
	}

	FT_Done_FreeType(ft);

	// shaders
	shader mShader("/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/ucore.vs", "/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/ucore.fs");
	shader uShader("/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/mcore.vs", "/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/mcore.fs");

	shader weighedShader("/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/weighedCore.vs", "/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/weighedCore.fs");

	shader textShader("/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/textCore.vs", "/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/textCore.fs");

	staticModel garbage(glm::vec3(0.0f, 0.0f, -6.0f), glm::vec3(1.0f), false);
	garbage.load("/Users/ulysses/Desktop/source/projects/game/source/assets/models/garbage/scene.gltf");

	ucube UCube = ucube(material::black_rubber, glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(1.0f));
	UCube.init();

	coolmanny.load("/Users/ulysses/Desktop/source/projects/game/source/assets/models/coolmanny4/coolmanny.gltf");

	m_spotLight uSpotLight = {
		camera::defaultCamera.cameraPosition, camera::defaultCamera.cameraFront,
		glm::cos(glm::radians(20.5f)), glm::cos(glm::radians(25.5f)),
		1.0f, 0.0f, 0.0f,
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f)
	};

	UTerrain.init();

	// render loop
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 textProjection = glm::mat4(1.0f);

	while (!Screen.shouldClose()) {
		
		// calculate deltaTime
		currentTime = glfwGetTime();

		//std::cout << currentTime << std::endl;

		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// process input
		processInput(deltaTime);

		// update screen
		Screen.update();

		view = glm::mat4(1.0f);
		projection = glm::mat4(1.0f);
		view = camera::defaultCamera.getViewMatrix();
		projection = glm::perspective(
			glm::radians(camera::defaultCamera.zoom), 
			(float)screen::SCR_WIDTH / (float)screen::SCR_HEIGHT, 0.1f, 100.0f
		);
		textProjection = glm::ortho(0.0f, (float)screen::SCR_WIDTH, 0.0f, (float)screen::SCR_HEIGHT);

		// lighting
		uSpotLight.position = camera::defaultCamera.cameraPosition;
		uSpotLight.direction = camera::defaultCamera.cameraFront;

		
		// render (send data to shaders)		

		weighedShader.activate();

		weighedShader.set3flt("viewPos", camera::defaultCamera.cameraPosition);

		weighedShader.set_flt("nBone", currentBone);
		weighedShader.setmat4("view", view);
		weighedShader.setmat4("projection", projection);

		coolmanny.currentAnim = currentAnim;
		coolmanny.render(weighedShader, currentTime);
		
		uSpotLight.render(weighedShader);

		uShader.activate();

		uShader.set3flt("viewPos", camera::defaultCamera.cameraPosition);
		
		uShader.setmat4("view", view);
		uShader.setmat4("projection", projection);

		uShader.setbool("blinn", blinn);
		uShader.setbool("gamma", gammaCorrection);

		uSpotLight.render(uShader);

		garbage.render(uShader);

		mShader.activate();

		mShader.set3flt("viewPos", camera::defaultCamera.cameraPosition);
		
		mShader.setmat4("view", view);
		mShader.setmat4("projection", projection);

		UCube.render(mShader);

		// text!!!
		textShader.activate();
		textShader.setmat4("projection", textProjection);
		testText.render(textShader, std::string("time: " + std::to_string(currentTime)), 10.0f, (float)screen::SCR_HEIGHT - 20.0f, glm::vec2(0.5f), glm::vec3(0.2f));
		testText.render(textShader, std::string("fps: " + std::to_string(1/deltaTime)), 10.0f, (float)screen::SCR_HEIGHT - 35.0f, glm::vec2(0.5f), glm::vec3(0.2f));



		//UTerrain.render(uShader);

		// send new frame to window
		Screen.newFrame();
		glfwPollEvents();
	}
	
	UTerrain.cleanUp();
	garbage.cleanUp();
	coolmanny.cleanUp();
	UCube.cleanUp();
	testText.cleanUp();

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
	if (keyboard::key(GLFW_KEY_Z)) {
		
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

	if (keyboard::keyWentDn(GLFW_KEY_B)) {
		
		blinn = !blinn;
	}

	if (keyboard::keyWentDn(GLFW_KEY_G)) {
		
		gammaCorrection = !gammaCorrection;
	}


	if (keyboard::keyWentDn(GLFW_KEY_O)) {
		
		currentAnim += 1;
	}

	if (keyboard::keyWentDn(GLFW_KEY_P)) {
		
		currentAnim -= 1;
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
	

	if (keyboard::key(GLFW_KEY_UP)) {

		coolmanny.position.x += 0.05f;
	}

	if (keyboard::key(GLFW_KEY_DOWN)) {
		
		coolmanny.position.x -= 0.05f;
	}

	if (keyboard::key(GLFW_KEY_RIGHT)) {

		coolmanny.position.z += 0.05f;
	}

	if (keyboard::key(GLFW_KEY_LEFT)) {
		
		coolmanny.position.z -= 0.05f;
	}
}
