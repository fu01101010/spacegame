#include "terrain.h"

terrain::terrain() { }

terrain::terrain(material Material, glm::vec3 position, const char* filename, float scale = 1.0f)
	: Material(Material), position(position), filename(filename), scale(scale) { }


bool terrain::loadHeightData(std::vector<unsigned char>& image, const std::string& filename, int& x, int& y) {

	int n; unsigned char* retval = stbi_load(filename.c_str(), &x, &y, &n, 4);
	
	if (retval != nullptr) {
		
		image = std::vector<unsigned char>(retval, retval + x * y * 4);
	}
	
	stbi_image_free(retval);
	
	return (retval != nullptr);
}


std::vector<float> terrain::createTerrain(int* nQuads) {

	std::vector<float> heightData = {};

	std::vector<float> Terrain;
    
    	int imageWidth, imageHeight;
    	std::vector<unsigned char> image;
    	bool success = loadHeightData(image, filename, imageWidth, imageHeight);
    	if (!success) { std::cout << "terrain::createTerrain::error_loading_image;" << std::endl; }

	*nQuads = imageWidth - 1;

	// fill heightData
	size_t index = 0; int RGBA = 4;
	for (int i = 0; i < imageWidth; ++i) {
		
		for (int j = 0; j < imageWidth; ++j) {
			
			index = RGBA * (i * imageWidth + j);
			heightData.push_back(static_cast<float>(image[index]) / 255.0f);
		}
	}

	// fill Terrain
	for (int i = 0; i < (imageWidth - 1); ++i) {
		
		for (int j = 0; j < (imageWidth - 1); ++j) {
			
			float bL = heightData.at(imageWidth*(i) + j);
			float bR = heightData.at(imageWidth*(i + 1) + j);
			float tR = heightData.at(imageWidth*(i + 1) + j + 1);
			float tL = heightData.at(imageWidth*(i) + j + 1);  

			Terrain.insert(Terrain.end(), {	
				//position					normal		 	aColor
			 	(float(i)   * scale), bL, (float(j)   * scale),	0.0f,  1.0f,  0.0f,	bL, bL, bL,
			 	(float(i+1) * scale), bR, (float(j)   * scale),	0.0f,  1.0f,  0.0f,	bR, bR, bR,
			 	(float(i+1) * scale), tR, (float(j+1) * scale),	0.0f,  1.0f,  0.0f,	tR, tR, tR,
			 	(float(i)   * scale), bL, (float(j)   * scale),	0.0f,  1.0f,  0.0f,	bL, bL, bL,
 			 	(float(i)   * scale), tL, (float(j+1) * scale),	0.0f,  1.0f,  0.0f,	tL, tL, tL,
			 	(float(i+1) * scale), tR, (float(j+1) * scale),	0.0f,  1.0f,  0.0f,	tR, tR, tR
			});
		}
	}
	
	return Terrain;
}


void terrain::init() {
	
	// in one dimension
	int nQuads = 0;
	
	std::vector<float> Terrain = createTerrain(&nQuads);
	unsigned int nVertices = 6 * nQuads * nQuads;

	float* vertices = &Terrain[0];

	std::vector<unsigned int> indices(nVertices);

	for (int i = 0; i < nVertices; ++i) {

		indices[i] = i;
	}

	meshes.push_back(u_mesh(u_vertex::genUList(vertices, nVertices), indices));
}

void terrain::render(shader Shader) {

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, position);

	Shader.setmat4("model", model);

	u_model::render(Shader);
}

