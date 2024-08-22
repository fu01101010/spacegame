cmake_minimum_required(VERSION 3.28)
project("spacegame")

find_package(OpenGL REQUIRED)

include_directories(
	# library headers
	"./include/"
    )

set( GLFW_SHARED_LIB
    "${PROJECT_SOURCE_DIR}/libs/glfw/lib-x86_64/libglfw.3.dylib"
)

set ( ALL_LIBS 
    ${OpenGL}
    ${GLFW_SHARED_LIB}
    "-ldl"
)

add_executable(game 
    "source/glad.c"
    "source/main.cpp"
    )

target_link_libraries(game ${ALL_LIBS})
install(TARGETS game DESTINATION "${PROJECT_SOURCE_DIR}/bin/")
