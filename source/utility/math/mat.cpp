#include "mat.h"

namespace mat {
	
	aiMatrix4x4 inverse(aiMatrix4x4& m) {
		
		m = GLMToAI(glm::inverse(AIToGLM(m)));

		return m;
	}

	glm::mat4 AIToGLM(aiMatrix4x4 m) {
		
		glm::mat4 mat = glm::mat4(m.a1, m.b1, m.c1, m.d1, 
					  m.a2, m.b2, m.c2, m.d2, 
					  m.a3, m.b3, m.c3, m.d3, 
					  m.a4, m.b4, m.c4, m.d4);

		return mat;
	}

	aiMatrix4x4 GLMToAI(glm::mat4 m) {
		
		aiMatrix4x4 mat = aiMatrix4x4(m[0][0], m[1][0], m[2][0], m[3][0], 
					      m[0][1], m[1][1], m[2][1], m[3][1], 
					      m[0][2], m[1][2], m[2][2], m[3][2], 
					      m[0][3], m[1][3], m[2][3], m[3][3]);
		
		return mat; 
	}

	void print(glm::mat4 m) {
		
		printf("%f, %f, %f, %f,\n", m[0][0], m[0][1], m[0][2], m[0][3]);
		printf("%f, %f, %f, %f,\n", m[1][0], m[1][1], m[1][2], m[1][3]);
		printf("%f, %f, %f, %f,\n", m[2][0], m[2][1], m[2][2], m[2][3]);
		printf("%f, %f, %f, %f,\n", m[3][0], m[3][1], m[3][2], m[3][3]);
	}

	void print(aiMatrix4x4 m) {
		
		printf("%f, %f, %f, %f,\n", m.a1, m.b1, m.c1, m.d1);
		printf("%f, %f, %f, %f,\n", m.a2, m.b2, m.c2, m.d2);
		printf("%f, %f, %f, %f,\n", m.a3, m.b3, m.c3, m.d3);
		printf("%f, %f, %f, %f,\n", m.a4, m.b4, m.c4, m.d4);
	}
}
