if(NOT DEFINED CMAKE_C_COMPILER_LAUNCHER AND NOT DEFINED CMAKE_CXX_COMPILER_LAUNCHER)
	find_program(CCACHE_FOUND ccache)
	find_program(SCCACHE_FOUND sccache)
	if (SCCACHE_FOUND)
		message("setting SCCACHE to ${SCCACHE_FOUND}")
		set(CMAKE_C_COMPILER_LAUNCHER ${SCCACHE_FOUND})
		set(CMAKE_CXX_COMPILER_LAUNCHER ${SCCACHE_FOUND})
	elseif(CCACHE_FOUND)
		message("setting CCACHE to ${CCACHE_FOUND}")
		set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_FOUND})
		set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_FOUND})
	endif()
endif()

cmake_minimum_required (VERSION 3.16)

project(opengl_samples)


if (${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	message(FATAL_ERROR "Compilation inside source folder not allowed. Use a build folder.")
endif()

set(CMAKE_CXX_STANDARD 			17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

# globally add GLB_DEBUG for the debug build
add_compile_definitions($<$<CONFIG:DEBUG>:GLB_DEBUG>)

# globally set -fno-strict-aliasing, needed due to using reinterpret_cast
if (NOT MSVC)
  add_compile_options(-fno-strict-aliasing)
endif()

if(MSVC AND (DEFINED CMAKE_C_COMPILER_LAUNCHER))
	message(DEBUG "Setting MSVC flags to /Z7 for ccache compatibility.  Current flags: ${CMAKE_CXX_FLAGS_DEBUG}")
	string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
	string(REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
	string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
	string(REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
	message(DEBUG "New flags: ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

# create bin folder
file(MAKE_DIRECTORY bin)

set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}/lib)

if(MSVC OR APPLE)
	set(RUNTIME_OUTPUT_DIRECTORY_DEBUG  	${BIN_DIR}/$<CONFIG>)
	set(RUNTIME_OUTPUT_DIRECTORY_RELEASE  	${BIN_DIR}/$<CONFIG>)
	set(LIBRARY_OUTPUT_DIRECTORY_DEBUG  	${BIN_DIR}/lib/$<CONFIG>)
	set(LIBRARY_OUTPUT_DIRECTORY_RELEASE  	${BIN_DIR}/lib/$<CONFIG>)
	set(ARCHIVE_OUTPUT_DIRECTORY_DEBUG  	${BIN_DIR}/lib/$<CONFIG>)
	set(ARCHIVE_OUTPUT_DIRECTORY_RELEASE  	${BIN_DIR}/lib/$<CONFIG>)
endif()

if(WIN32)
	set(VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

include(sample_helpers)
include(utils)

# Add thirdparty libraries
add_subdirectory(thirdparty)

# add opengl framework
add_subdirectory(framework)

#build samples
add_subdirectory(samples)