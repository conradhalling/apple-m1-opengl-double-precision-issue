FetchContent_Declare(
    glew
    URL https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.zip
)

FetchContent_GetProperties(glew)
if(NOT glew_POPULATED)
    FetchContent_Populate(glew)
    add_subdirectory(${glew_SOURCE_DIR}/build/cmake)
endif()