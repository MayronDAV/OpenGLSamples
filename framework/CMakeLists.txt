cmake_minimum_required(VERSION 3.16)

project(framework LANGUAGES C CXX)

set(SOURCE_DIR      			"${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCE_FILES
								"${SOURCE_DIR}/**.h"
								"${SOURCE_DIR}/**.cpp"
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${GLFW_INCLUDE_DIR}
        ${GLAD_INCLUDE_DIR}
        ${STB_DIR}
        ${GLM_INCLUDE_DIR}
        ${FMT_INCLUDE_DIR}
)

target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC
        glfw
        glm
        stb
        fmt
        ${OPENGL_LIBS}
        glad
)