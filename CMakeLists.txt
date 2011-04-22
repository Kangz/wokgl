cmake_minimum_required(VERSION 2.6)

project(renderer)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include_directories(
    include/SDL
    include
)

link_directories(
    ${CMAKE_SOURCE_DIR}/extlibs
)

file(
	GLOB_RECURSE
	renderer_sources
	src/renderer/*
)

add_executable(
    test_normalmap
    src/test_normalmap.cpp
    ${renderer_sources}
)

target_link_libraries(
	test_normalmap
	SDL
	GLEW
	GL
)

#g++ -c include/glm/glm.hpp #Compiles glm


#g++ -Iinclude/SDL -Iinclude -D_GNU_SOURCE=1 -D_REENTRANT -c test.cpp
#g++ -Lextlibs -o test test.o -lSDL -lGLEW -lGL

