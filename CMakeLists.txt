cmake_minimum_required(VERSION 2.8)

project(renderer)

#GLM has triggers the long-long warning
SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -Wextra -pedantic -Wno-long-long -Wno-unused-parameter -Wno-unused-variable")

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

add_executable(
    test_framebuffer
    src/test_framebuffer.cpp
    ${renderer_sources}
)


target_link_libraries(
	test_normalmap
	SDL2
	SDL2_image
    pthread
    dl
	GLEW
	GL
)

target_link_libraries(
	test_framebuffer
	SDL2
	SDL2_image
	GLEW
	GL
    pthread
    dl
)
