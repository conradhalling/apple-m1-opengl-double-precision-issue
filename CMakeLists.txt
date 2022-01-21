cmake_minimum_required(VERSION 3.16)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(main)

set(CMAKE_CXX_STANDART 14)

include(FetchContent)
include(glfw)
include(glew)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

# We include our source files here
add_executable(main src/main.cpp)

# Link the necessary libraries
find_package(OpenGL REQUIRED)
target_include_directories(main PRIVATE ${glew_SOURCE_DIR}/include)
target_link_libraries(main PRIVATE OpenGL::GL)
target_link_libraries(main PRIVATE glfw glew)