#ifndef TERRAIN_H
#define TERRAIN_H

#include <iostream>
#include <vector>

#include <stb/stb_image.h>

#include "../../graphics/utility/model.h"
#include "../../graphics/utility/material.h"

class terrain : public u_model {
	
public:

	glm::vec3 position;
	float scale;

	material Material;

	const char* filename;

	terrain();
	terrain(material Material, glm::vec3 position, const char* filename, float scale);

	bool loadHeightData(std::vector<unsigned char>& image, const std::string& filename, int& x, int& y);

	std::vector<float> createTerrain(int* nQuads);

	void init();

	void render(shader Shader);
};

#endif
