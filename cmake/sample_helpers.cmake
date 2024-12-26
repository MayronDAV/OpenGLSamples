function(add_project target)

	project(${target} LANGUAGES C CXX)

	file(GLOB_RECURSE SAMPLE_SRC_FILES 
				"${CMAKE_CURRENT_SOURCE_DIR}/src/**.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp"
	)

	add_executable(${PROJECT_NAME} ${SAMPLE_SRC_FILES})

	set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

	set_target_properties(${target} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}/$<CONFIG>/${target}
	)

	if(MSVC OR APPLE)
		set_target_properties(${target} PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY_DEBUG  	${BIN_DIR}/$<CONFIG>/${target}
			RUNTIME_OUTPUT_DIRECTORY_RELEASE  	${BIN_DIR}/$<CONFIG>/${target}
		)
	endif()

	target_include_directories(
		${PROJECT_NAME} 
		PUBLIC 
			${CMAKE_CURRENT_SOURCE_DIR}/src
			${GLM_INCLUDE_DIR}
	)

	target_link_libraries(${PROJECT_NAME} PUBLIC framework)

endfunction()