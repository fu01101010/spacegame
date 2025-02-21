#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <glm/glm.hpp>

#include <ft2build.h>
#include <freetype/freetype.h>

#include "../../graphics/shader.h"
#include "../../graphics/light.h"

#include "../../graphics/text/text.h"


#include "../../io/camera.h"
#include "../../io/keyboard.h"
#include "../../io/mouse.h"

class scene {

public:

	// constructor
	scene();
	scene(int glfwCVMajor, int glfwCVMinor, const char* title, unsigned int _width, unsigned int _height); // scrwidth, scrheight
	

	// callbacks
	
	// window resize
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);


	// init method
	bool init();

	
	// main loop methods
	void processInput(double deltaTime);

	void update(); // update screen before each frame

	void newFrame(); // update screen after each frame

	void render(shader Shader); // set vars + render

	void cleanUp();


	// accessor funcs
	bool shouldClose();
	
	// modifier funcs
	void setShouldClose(bool shouldCloseFlag);

	void setWindowColor(float r, float g, float b, float a);

	
	// lights
	std::vector<pointLight*> pointLights;
	unsigned int activePointLights;

	std::vector<spotLight*> spotLights;
	unsigned int activeSpotLights;

	std::vector<directLight*> directLights;
	unsigned int activeDirectLights;


	// camera
	std::vector<camera*> cameras;
	unsigned int activeCamera;

	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 cameraPosition;

	FT_Library ft;
	ftText glyph;
protected:

	// window object
	GLFWwindow* window;

	// window values
	const char* title;
	static unsigned int width;
	static unsigned int height;
	float color[4];

	// GLFW
	int glfwCVMajor;
	int glfwCVMinor;
};

#endif
