#version 330 core

layout (location = 0) in vec3 vsInPosition;
layout (location = 1) in vec3 vsInNormal;
layout (location = 2) in vec3 vsInColor;
layout (location = 3) in vec2 vsInTextureCoords;
layout (location = 4) in vec4 vsInWVBones;
layout (location = 5) in vec4 vsInWeights;

out vec3 vsOutFragPos;
out vec3 vsOutNormal;
out vec3 vsOutColor;
out vec2 vsOutTextureCoords;
out vec4 vsOutWVBones;
out vec4 vsOutWeights;

const int MAX_BONES = 26;
uniform mat4 boneData[MAX_BONES];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	mat4 boneTransform = boneData[int(vsInWVBones[0])] * vsInWeights[0];
	boneTransform += boneData[int(vsInWVBones[1])] * vsInWeights[1];
	boneTransform += boneData[int(vsInWVBones[2])] * vsInWeights[2];
	boneTransform += boneData[int(vsInWVBones[3])] * vsInWeights[3];

	vsOutFragPos	= vec3(model * boneTransform * vec4(vsInPosition, 1.0));
	vsOutNormal	= mat3(transpose(inverse(model))) * vsInNormal;

	gl_Position = projection * view * vec4(vsOutFragPos, 1.0);
	
	vsOutColor = vsInColor;
	vsOutTextureCoords = vsInTextureCoords;

	vsOutWVBones = vsInWVBones;
	vsOutWeights = vsInWeights;
}
