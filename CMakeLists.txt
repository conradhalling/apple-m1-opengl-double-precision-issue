cmake_minimum_required(VERSION 3.16)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(main)

include(FetchContent)
include(glfw)
include(glew)
find_package(OpenGL REQUIRED)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

add_executable(main)
target_sources(main PRIVATE src/main.cpp)
target_include_directories(main PRIVATE ${glew_SOURCE_DIR}/include)
target_compile_features(main PRIVATE cxx_std_14)
target_link_libraries(main PRIVATE OpenGL::GL glfw glew)