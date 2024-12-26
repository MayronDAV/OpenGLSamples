cmake_minimum_required(VERSION 3.10)

project(thirdparty LANGUAGES C CXX)

set(OPENGL_LIBS )

if(WIN32)
	list(APPEND OPENGL_LIBS opengl32)
  	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(UNIX AND NOT APPLE)
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
	find_package(OpenGL REQUIRED)
	add_definitions(${OPENGL_DEFINITIONS})
	find_package(X11 REQUIRED)
	list(APPEND OPENGL_LIBS X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread)
	set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(APPLE)
	set(FIND_PATH "/System/Library/Frameworks")
	find_library(COCOA_LIBRARY Cocoa PATHS FIND_PATH)
	find_library(OpenGL_LIBRARY OpenGL PATHS FIND_PATH)
	find_library(IOKit_LIBRARY IOKit PATHS FIND_PATH)
	find_library(CoreVideo_LIBRARY CoreVideo PATHS FIND_PATH)
	mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
	list(APPEND OPENGL_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
endif()

# GLAD
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glad)
set(GLAD_INCLUDE_DIR ${GLAD_DIR}/include)
add_library(glad INTERFACE)
target_sources(glad INTERFACE ${GLAD_DIR}/src/glad.c)
target_include_directories(glad SYSTEM INTERFACE ${GLAD_INCLUDE_DIR})
set_property(TARGET glad PROPERTY FOLDER "thirdparty")

# GLM
add_library(glm INTERFACE)
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm)
set(GLM_INCLUDE_DIR ${GLM_DIR}/include)
target_sources(glm INTERFACE)
target_include_directories(glm SYSTEM INTERFACE ${GLM_INCLUDE_DIR})

target_compile_definitions(glm INTERFACE
    GLM_FORCE_SWIZZLE
    GLM_FORCE_RADIANS
    GLM_FORCE_CTOR_INIT
    GLM_ENABLE_EXPERIMENTAL
)
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_definitions(glm INTERFACE GLM_FORCE_CXX14)
endif()
set_property(TARGET glm PROPERTY FOLDER "thirdparty")

# STB
add_library(stb INTERFACE)
set(STB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stb)
target_include_directories(stb SYSTEM INTERFACE ${STB_DIR})
set_property(TARGET stb PROPERTY FOLDER "thirdparty")

# GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)
option(BUILD_SHARED_LIBS ON)
if (VKB_WSI_SELECTION STREQUAL WAYLAND)
    set(GLFW_USE_WAYLAND ON)
endif()

add_subdirectory(glfw)
set(GLFW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)

if (TARGET update_mappings)
    set_target_properties(update_mappings PROPERTIES FOLDER "thirdparty")
endif()
set_property(TARGET glfw PROPERTY FOLDER "thirdparty")

# fmt
add_subdirectory(fmt)
set(FMT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fmt/include)
set_property(TARGET fmt PROPERTY FOLDER "thirdparty")
