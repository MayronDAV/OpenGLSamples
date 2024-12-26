function(add_project target)

	project(${target} LANGUAGES C CXX)

	file(GLOB_RECURSE SAMPLE_SRC_FILES 
				"${CMAKE_CURRENT_SOURCE_DIR}/src/**.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp"
	)

	add_executable(${PROJECT_NAME} ${SAMPLE_SRC_FILES})

	set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

	target_include_directories(
		${PROJECT_NAME} 
		PUBLIC 
			${CMAKE_CURRENT_SOURCE_DIR}/src
			${GLM_INCLUDE_DIR}
	)

	target_link_libraries(${PROJECT_NAME} PUBLIC framework)

endfunction()