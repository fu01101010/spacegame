#include "text.h"

ftText::ftText() {}
ftText::ftText(unsigned int height) 
	: height(height) {}

bool ftText::load(FT_Library &ft, std::string path) {
	
	FT_Face fontFace;

	if (FT_New_Face(ft, path.c_str(), 0, &fontFace)) {
		
		return false;
	}

	// set height and width
	FT_Set_Pixel_Sizes(fontFace, 0, height); // width will resize dynamically
	
	// load texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disables byte-alignement restriction
	
	for (unsigned char c = 0; c < 128; c++) { //c++!!! =:)
		
		// load glyph
		if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER)) {
			
			continue;
		}

		// generate texture
		GLuint texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RED, 
			fontFace->glyph->bitmap.width,
			fontFace->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE,
			fontFace->glyph->bitmap.buffer
		);

		// set texture params
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// store character
		characterMap[c] = {texture, 
			glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
	       		glm::ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
			(unsigned int)(fontFace->glyph->advance.x)
		};
	}

	FT_Done_Face(fontFace);

	// set VAO&VBO
	// using glMemoryBuffers
	//VAO.generate();
	//VAO.bind();
	//VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);
	//VAO["VBO"].generate();
	//VAO["VBO"].bind();
	
	// 2D quad requires 6 vertices of 4 float (2 for pos & 2 for texcoord)
	//VAO["VBO"].setData<GLfloat>(6 * 4, nullptr, GL_DYNAMIC_DRAW);
	//VAO["VBO"].setAttPointer<GLfloat>(0, 4, GL_FLOAT, 4, 0);
	//VAO["VBO"].clear();
	//ArrayObject::clear();
	
	// will do manually for now.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);

	// set pointers here
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(GLfloat)));
	

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//error

	return true;
}

void ftText::render(shader Shader, std::string text, float x, float y, glm::vec2 scale, glm::vec3 color) {
	
	//error
	Shader.activate(); //REMOVE
	Shader.set3flt("textColor", color);

	glActiveTexture(GL_TEXTURE0);
	
	//error
	//VAO.bind();
	
	glBindVertexArray(VAO);

	// go through all characters
	for (unsigned int i = 0, len = text.size(); i < len; ++i) {
		
		character c = characterMap[text[i]];

		float xPos = x + c.bearing.x * scale.x;
		float yPos = y - (c.size.y - c.bearing.y) * scale.y;

		float width = c.size.x * scale.x;
		float height = c.size.y * scale.y;

		// new VBO data
		float vertices[6 * 4] = {
		//	x		y		texx	texy
			xPos,		yPos + height,	0.0f,	0.0f,
			xPos,		yPos,		0.0f,	1.0f,
			xPos + width,	yPos,		1.0f,	1.0f,

			xPos,		yPos + height,	0.0f,	0.0f,
			xPos + width,	yPos,		1.0f,	1.0f,
			xPos + width,	yPos + height,	1.0f,	0.0f
		};

		// set up the texture
		glBindTexture(GL_TEXTURE_2D, c.textureID);

		// update VBO
		//VAO["VBO"].bind();
		//VAO["VBO"].updateData<GLfloat>(0, 6 * 4, vertices);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * 4 * sizeof(GLfloat), vertices);

		// render quad
		//VAO.draw(GL_TRIANGLES, 0, 6);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// advance cursor
		x += ((unsigned int)c.advance >> 6) * scale.x; // multiply by 64
	}

	//error
	//ArrayObject::clear();
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void ftText::cleanUp() {

	//error
	
	//VAO.cleanUp();
	glDeleteVertexArrays(1, &VAO);
}
