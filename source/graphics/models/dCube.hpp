#ifndef VERT_CUBE_HPP
#define VERT_CUBE_HPP

#include "../DFLT_Model.h"
#include "../../Material.h"

class DFLT_CUBE : public DFLT_MODEL {

public:

	glm::vec3 Position;
	glm::vec3 Size;

	MATERIAL Material;

	DFLT_CUBE(glm::vec3 Position = glm::vec3(0.0f), glm::vec3 Size = glm::vec3(1.0f)) 
		: DFLT_MODEL(Position, Size) {}

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
		/*
		TEXTURE TEX0("assets/textures/cool_antenna.jpg", "Material.Diffuse");
		TEX0.LOAD();
		TEXTURE TEX1("assets/textures/cool_shuttle.png", "Material.Specular");
		TEX1.LOAD();
		*/

		MESHES.push_back(DFLT_MESH(VERTEX::GENList(VERTICES, NVERTICES), INDICES));
	}
};

#endif