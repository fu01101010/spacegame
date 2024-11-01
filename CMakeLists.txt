cmake_minimum_required(VERSION 3.28)
project("spacegame")

find_package(OpenGL REQUIRED)

include_directories(
	# library headers
	"./include/"
)

set( CMAKE_CXX_STANDARD 11) 

set( GLFW_SHARED_LIB
	"${PROJECT_SOURCE_DIR}/libs/glfw/lib-x86_64/libglfw.3.dylib"
)

set( ASSIMP_SHARED_LIB
	"${PROJECT_SOURCE_DIR}/libs/assimp/lib-x86_64/libassimp.5.dylib"
)

set ( STB_IMAGE
	"${PROJECT_SOURCE_DIR}/libs/stb/stb_image.cpp"	
)

set ( ALL_LIBS 
	${OpenGL}
	${GLFW_SHARED_LIB}
	${ASSIMP_SHARED_LIB}
	${STB_IMAGE}
	"-ldl"
)

add_executable(game 
	"source/glad.c"
	"source/graphics/light.cpp"
	"source/graphics/material.cpp"
	"source/graphics/shader.cpp"
	"source/graphics/mesh.cpp"
	"source/graphics/texture.cpp"
	"source/graphics/models/model.cpp"
	"source/graphics/models/cube.hpp"
	"source/io/camera.cpp"
	"source/io/keyboard.cpp"
	"source/io/mouse.cpp"
	"source/io/screen.cpp"
	"source/main.cpp"
)

target_link_libraries(game ${ALL_LIBS})
install(TARGETS game DESTINATION "${PROJECT_SOURCE_DIR}/bin/")
