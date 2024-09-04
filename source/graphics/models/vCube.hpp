#ifndef VERT_CUBE_HPP
#define VERT_CUBE_HPP

#include "../VERT_Model.h"
#include "../../Material.h"

class VERT_CUBE : public VERT_MODEL {

public:

	glm::vec3 Position;
	glm::vec3 Size;

	MATERIAL Material;

	VERT_CUBE() {}
	VERT_CUBE(MATERIAL Material, glm::vec3 Position, glm::vec3 Size) 
		: Material(Material), Position(Position), Size(Size) {}

	void INIT() {

		unsigned int NVERTICES = 36;

		float VERTICES[] = {
			//Position				Normal					TexCoord
			-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,	1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,	0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,	0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,	0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,	0.0f, 1.0f
		};

		std::vector<unsigned int> INDICES(NVERTICES);

		for (int i = 0; i < NVERTICES; ++i) {

			INDICES[i] = i;
		}

		VERT_TEXTURE TEX0("assets/textures/cool_antenna.jpg", "Material.Diffuse");
		TEX0.LOAD();
		VERT_TEXTURE TEX1("assets/textures/cool_shuttle.png", "Material.Specular");
		TEX1.LOAD();

		MESHES.push_back(VERT_MESH(VERTEX::GENList(VERTICES, NVERTICES), INDICES, {TEX0, TEX1}));
	}

	void RENDER(SHADER SHADER) {

		glm::mat4 MODEL = glm::mat4(1.0f);

		MODEL = glm::translate(MODEL, Position);
		MODEL = glm::scale(MODEL, Size);
		//MODEL = glm::rotate(MODEL, (float)glfwGetTime() * glm::radians(-45.0f), glm::vec3(0.5f));

		SHADER.SETMAT4("MODEL", MODEL);
		SHADER.SET3FLT("Material.Ambient", Material.Ambient);
		//SHADER.SET3FLT("Material.Diffuse", Material.Diffuse);
		//SHADER.SET3FLT("Material.Specular", Material.Specular);
		SHADER.SET_FLT("Material.Reflectivity", Material.Reflectivity);

		VERT_MODEL::RENDER(SHADER);
	}
};

#endif