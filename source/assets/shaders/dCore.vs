#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 vsOutFragPos;
out vec3 vsOutNormal;
out vec2 vsOutTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	vsOutFragPos	= vec3(model * vec4(aPos, 1.0));
	vsOutNormal	= mat3(transpose(inverse(model))) * aNormal;

	gl_Position = projection * view * vec4(vsOutFragPos, 1.0);
	
	vsOutTexCoord = aTexCoord;
}
