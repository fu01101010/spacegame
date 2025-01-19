#version 330 core

in vec3 vsOutFragPos;
in vec3 vsOutNormal;
in vec3 vsOutColor;
in vec4 vsOutWVBones;
in vec4 vsOutWeights;

uniform vec3 viewPos;

uniform float nBone;

out vec4 fragmentColor;

void main() {

	vec3 normal	= normalize(vsOutNormal);
	vec3 viewDir	= normalize(viewPos - vsOutFragPos);

	
	bool found = false;
	for (int i = 0; i < 4; i++) {
	
		if (vsOutWVBones[i] == nBone) {

			found = true;

			if (vsOutWeights[i] >= 0.66) {

				fragmentColor = vec4(1.0, 0.0, 0.0, 0.0);
			} else if (vsOutWeights[i] > 0.33 && vsOutWeights[i] <= 0.66) {

				fragmentColor = vec4(0.0, 1.0, 0.0, 0.0);
			} else if (vsOutWeights[i] > 0.0) {

				fragmentColor = vec4(1.0, 1.0, 0.0, 0.0);
			}
		}
	}

	if (!found) {
		fragmentColor = vec4(0.0, 0.0, 1.0, 0.0);
	}
}

