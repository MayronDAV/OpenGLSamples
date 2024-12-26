cmake_minimum_required(VERSION 3.16)

# Creates a list of the subdirectories within this folder
scan_dirs(${CMAKE_CURRENT_SOURCE_DIR} SUB_DIRS)


function(add_sub_dirs DIRECTORY)
	if (EXISTS "${DIRECTORY}/CMakeLists.txt")
		message(STATUS "Founded sample: ${DIRECTORY}")
		add_subdirectory(${DIRECTORY})
	else ()
		scan_dirs(${DIRECTORY} DIR_LIST)

		foreach (CURR_DIR ${DIR_LIST})
			if (EXISTS "${DIRECTORY}/${CURR_DIR}/CMakeLists.txt")
				message(STATUS "Founded sample: ${DIR}/${CHILD}")
				add_subdirectory(${DIRECTORY}/${CURR_DIR})
			else ()
				add_sub_dirs(${DIRECTORY}/${CURR_DIR})
			endif ()
		endforeach ()
	endif()
endfunction(add_sub_dirs)

foreach (SUB_DIR ${SUB_DIRS})
    add_sub_dirs(${CMAKE_CURRENT_SOURCE_DIR}/${SUB_DIR})
endforeach ()
