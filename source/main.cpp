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

#include "../source/utility/scene/scene.h"
#include "../source/utility/terrain/terrain.h"
#include "../source/utility/math/mat.h"


// funcs here
void processInput(double deltaTime);


// io here

double dx, dy;
double scrollDX, scrollDY;

double deltaTime = 0.0f; // time inbetween frames
double lastFrame = 0.0f; // time of last frame
double currentTime = 0.0f;

// temporary 
camera cam(glm::vec3(0.0f));

bool wireframeFlag = false;

terrain UTerrain = terrain(material::black_plastic, glm::vec3(0.0f), "/Users/ulysses/Desktop/source/projects/game/source/assets/heightMaps/image.jpg", 0.01f);

int currentBone = 6;

int currentAnim = 0;

weighedModel coolmanny(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), false);

bool blinn = false;
bool gammaCorrection = false;

//FT_Library ft;
//ftText text = ftText(32);

scene Scene;

// main
int main() {

	std::cout << "Hello, endless space!" << std::endl;

	Scene = scene(3, 3, "game", 800, 600);

	
	if (!Scene.init()) {

		std::cout << "failed to open window" << std::endl;
		glfwTerminate();

		return -1;
	}

	// shaders
	shader mShader("/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/ucore.vs", "/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/ucore.fs");
	shader uShader("/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/mcore.vs", "/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/mcore.fs");

	shader weighedShader("/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/weighedCore.vs", "/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/weighedCore.fs");

	shader textShader("/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/textCore.vs", "/Users/ulysses/Desktop/source/projects/game/source/assets/shaders/core/textCore.fs");

	staticModel garbage(glm::vec3(0.0f, 0.0f, -6.0f), glm::vec3(1.0f), false);
	garbage.load("/Users/ulysses/Desktop/source/projects/game/source/assets/models/garbage/scene.gltf");

	ucube UCube = ucube(material::black_rubber, glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(1.0f));
	UCube.init();

	Scene.cameras.push_back(&cam);
	Scene.activeCamera = 0;

	coolmanny.load("/Users/ulysses/Desktop/source/projects/game/source/assets/models/coolmanny4/coolmanny.gltf");

	spotLight SpotLight = {
		cam.cameraPosition, cam.cameraFront,
		glm::cos(glm::radians(20.5f)), glm::cos(glm::radians(25.5f)),
		1.0f, 0.0f, 0.0f,
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f)
	};

	Scene.spotLights.push_back(&SpotLight);
	Scene.activeSpotLights = 1;

	UTerrain.init();

	// render loop
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 textProjection = glm::mat4(1.0f);

	while (!Scene.shouldClose()) {
		
		// calculate deltaTime
		currentTime = glfwGetTime();

		//std::cout << currentTime << std::endl;

		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		
		// update screen
		Scene.update();

		// process the rest of the input
		processInput(deltaTime);
		
		/*
		view = glm::mat4(1.0f);
		projection = glm::mat4(1.0f);
		view = cam.getViewMatrix();
		projection = glm::perspective(
			glm::radians(cam.zoom), 
			(float)screen::SCR_WIDTH / (float)screen::SCR_HEIGHT, 0.1f, 100.0f
		);
		*/
		
		textProjection = glm::ortho(0.0f, (float)screen::SCR_WIDTH, 0.0f, (float)screen::SCR_HEIGHT);

		// lighting
		SpotLight.position = cam.cameraPosition;
		SpotLight.direction = cam.cameraFront;

		// render (send data to shaders)		

		weighedShader.activate();		
		weighedShader.set_flt("nBone", currentBone);		

		Scene.render(weighedShader);
		coolmanny.currentAnim = currentAnim;
		coolmanny.render(weighedShader, currentTime);
		

		uShader.activate();
		uShader.setbool("blinn", blinn);
		uShader.setbool("gamma", gammaCorrection);

		Scene.render(uShader);
		garbage.render(uShader);


		Scene.render(mShader);
		UCube.render(mShader);

		// text!!!
		textShader.activate();
		textShader.setmat4("projection", textProjection);
		Scene.glyph.render(textShader, std::string("time: " + std::to_string(currentTime)), 10.0f, (float)screen::SCR_HEIGHT - 20.0f, glm::vec2(0.5f), glm::vec3(0.2f));
		Scene.glyph.render(textShader, std::string("fps: " + std::to_string(1/deltaTime)), 10.0f, (float)screen::SCR_HEIGHT - 35.0f, glm::vec2(0.5f), glm::vec3(0.2f));

		//UTerrain.render(uShader);

		// send new frame to window		
		Scene.newFrame();
	}
	
	UTerrain.cleanUp();
	garbage.cleanUp();
	coolmanny.cleanUp();
	UCube.cleanUp();

	Scene.cleanUp();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly

void processInput(double deltaTime) {
	
	Scene.processInput(deltaTime);

	if (keyboard::key(GLFW_KEY_ESCAPE)) {
		
		Scene.setShouldClose(true);
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
