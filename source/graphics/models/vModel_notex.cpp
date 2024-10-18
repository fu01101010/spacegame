#include "vModel_notex.h"

vModel_notex::vModel_notex() {}

void vModel_notex::init() {}

void vModel_notex::render(shader Shader) {

	for (vMesh_notex mesh : vMeshes) {

		mesh.render(Shader);
	}
}

void vModel_notex::cleanUp() {

	for (vMesh_notex mesh : vMeshes) {

		mesh.cleanUp();
	}
}
