function(scan_dirs DIR LIST)
    file(GLOB CHILD_DIRS RELATIVE ${DIR} ${DIR}/*)

    set(FILTERED_DIRS "")

    foreach (CHILD ${CHILD_DIRS})
        if (IS_DIRECTORY "${DIR}/${CHILD}")
            list(APPEND FILTERED_DIRS ${CHILD})
        endif ()
    endforeach ()

    set(${LIST} ${FILTERED_DIRS} PARENT_SCOPE)
endfunction()

function(copy_file src dest target)
	if(WIN32)
		add_custom_command(
			TARGET ${target} 
			PRE_BUILD 
			COMMAND ${CMAKE_COMMAND} -E copy ${src} ${dest} 
			COMMENT "Copying file: ${src} -> ${dest}"
		)
	elseif(UNIX AND NOT APPLE)
		add_custom_command(
			TARGET ${target}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy ${src} ${dest}
			COMMENT "Copying file (Unix): ${src} -> ${dest}"
		)
	elseif(APPLE)
		add_custom_command(
			TARGET ${target} 
			POST_BUILD 
			COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}  
			COMMENT "Creating symbolic link: ${src} -> ${dest}"
		)
	endif()
endfunction()