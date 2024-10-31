#ifndef CUBE_HPP
#define CUBE_HPP

#include <vector>
#include <string>

#include "model.h"
#include "../material.h"

class cube : public u_model {

public:

	glm::vec3 position;
	glm::vec3 size;

	material Material;

	bool texFlag;

	// <-- tex directories listed manually
	std::vector<std::string> texes = {};

	cube() {}
	cube(material Material, glm::vec3 position, glm::vec3 size, bool texFlag = false) 
		: Material(Material), position(position), size(size), texFlag (texFlag) {}

	cube(material Material, glm::vec3 position, glm::vec3 size, bool texFlag, std::vector<std::string> texes)
		: Material(Material), position(position), size(size), texFlag (texFlag), texes (texes) {}

	void init() {

		unsigned int nVertices = 36;

		float vertices[] = {
			//position		Normal			TexCoord
			-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,

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

		std::vector<unsigned int> indices(nVertices);

		for (int i = 0; i < nVertices; ++i) {

			indices[i] = i;
		}

		if (texFlag) {
			v_texture tex0("/Users/ulysses/Desktop/source/projects/game/source//assets/textures/cool_antenna.jpg", "Material.diffuse");
			tex0.load();
			v_texture tex1("/Users/ulysses/Desktop/source/projects/game/source/assets/textures/cool_shuttle.png", "Material.specular");
			tex1.load();

			meshes.push_back(mesh(vertex::genList(vertices, nVertices), indices, {tex0, tex1}));
		} else {
			
			meshes.push_back(mesh(vertex::genList(vertices, nVertices), indices));
		}

	}

	void render(shader Shader) {

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, position);
		model = glm::scale(model, size);
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-45.0f), glm::vec3(0.5f));

		Shader.setmat4("model", model);
		//Shader.set3flt("Material.ambient", Material.ambient);
		Shader.set_int("Material.diffuse", meshes[0].textures[0].texID);
		Shader.set_int("Material.specular", meshes[0].textures[1].texID);
		Shader.set_flt("Material.reflectivity", Material.reflectivity);

		model::render(Shader);
	}
};

#endifte
