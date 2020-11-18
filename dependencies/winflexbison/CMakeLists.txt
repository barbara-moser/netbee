if (WIN32)
	include(FetchContent)

	FetchContent_Declare(
		winflexbison
		URL "${CMAKE_CURRENT_SOURCE_DIR}/winflexbison-master.zip"
		)

	FetchContent_GetProperties(winflexbison)
	if (NOT winflexbison_POPULATED)
		FetchContent_Populate(winflexbison)
		add_subdirectory("${winflexbison_SOURCE_DIR}" "${winflexbison_BINARY_DIR}")
	endif()
	foreach(target win_bison win_flex winflexbison_common)
		set_target_properties(${target} PROPERTIES FOLDER "Dependencies/WinFlexBison")
	endforeach()
else()
	find_program(BISON_EXECUTABLE bison)
	if (NOT BISON_EXECUTABLE)
		message(FATAL_ERROR "Could not find bison")
	endif()
	add_executable(win_bison IMPORTED GLOBAL)
	set_target_properties(win_bison PROPERTIES
		IMPORTED_LOCATION ${BISON_EXECUTABLE}
	)
	unset(BISON_EXECUTABLE)

	find_program(FLEX_EXECUTABLE flex)
	if (NOT FLEX_EXECUTABLE)
		message(FATAL_ERROR "Could not find flex")
	endif()
	add_executable(win_flex IMPORTED GLOBAL)
	set_target_properties(win_flex PROPERTIES
		IMPORTED_LOCATION ${FLEX_EXECUTABLE}
	)
	unset(FLEX_EXECUTABLE)
endif()
