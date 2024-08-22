#########
spacegame
#########

1: project layout
=================

- :code:`root`: The :code:`CMakeLists.txt` and :code:`.command` files are located directly in the root folder.

  - :code:`/bin`: contains the :code:`game` executable file.

  - :code:`/build`: contains the :code:`cmake` generated build files.

  - :code:`/source`: contains the :code:`main.cpp` source file and other headers.

2: cmake
========

.. code-block::

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

3: build
========

- :code:`game.command` contains the following:

  - :code:`~/projectRoot rm -rf build/` and :code:`~/projectRoot mkdir build` basically clears the build directory.

  - :code:`cd build`: enters the directory.

  - :code:`cmake ..`: builds the :code:`CMakeLists.txt` located in the parent directory.

  - :code:`make install`: creates the :code:`game` executable and puts it in the :code:`~/projectRoot/bin` directory.

  - :code:`cd ../bin`: enters the /bin directory.

  - :code:`./game`: launches the executable.