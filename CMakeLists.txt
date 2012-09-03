cmake_minimum_required(VERSION 2.8)

project(renderer)

#GLM has triggers the long-long warning
SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -Wextra -pedantic -Wno-long-long -Wno-unused-parameter -Wno-unused-variable")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(
    include/SDL
    include
)

link_directories(
    ${CMAKE_SOURCE_DIR}/extlibs
)

#file sets
file(
	GLOB_RECURSE
	renderer_sources
	src/renderer/*
)

#build static libraries
add_library(
    renderer STATIC
    ${renderer_sources}
)
target_link_libraries(renderer SDL2 SDL2_image dl GLEW GL)

#make executables
add_executable(
    test_normalmap
    src/test_normalmap.cpp
)

#add_executable(
#    test_framebuffer
#    src/test_framebuffer.cpp
#)

target_link_libraries(
	test_normalmap
	SDL2
    pthread
    dl
    renderer
)

#target_link_libraries(
#    test_framebuffer
#    SDL2
#    pthread
#    dl
#    renderer
#)
