#version 330 core

layout (location = 0) in vec3 vsInPosition;
layout (location = 1) in vec3 vsInNormal;
layout (location = 2) in vec3 vsInColor;
layout (location = 3) in vec2 vsInTextureCoords;

out vec3 vsOutFragPos;
out vec3 vsOutNormal;
out vec3 vsOutColor;
out vec2 vsOutTextureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	vsOutFragPos	= vec3(model * vec4(vsInPosition, 1.0));
	vsOutNormal	= mat3(transpose(inverse(model))) * vsInNormal;

	gl_Position = projection * view * vec4(vsOutFragPos, 1.0);
	
	vsOutColor = vsInColor;
	vsOutTextureCoords = vsInTextureCoords;
}
