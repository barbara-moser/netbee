include(FetchContent)

FetchContent_Declare(
	netbeelib
	GIT_REPOSITORY https://github.com/barbara-moser/netbee
	GIT_TAG 5d34842634d0b02fef0c9e8da8d29db67afe6287
	)

FetchContent_GetProperties(netbeelib)
if (NOT netbeelib_POPULATED)
	FetchContent_Populate(netbeelib)
	add_subdirectory(${netbeelib_SOURCE_DIR} ${netbeelib_BINARY_DIR})

	add_library(netbee INTERFACE)
	target_link_libraries(netbee INTERFACE nbee)

	add_custom_target(netbee_copy_files)
	add_dependencies(netbee netbee_copy_files)

	foreach(target nbee nbpflcompiler nbprotodb nbsockutils nbnetvm pcre pcrecpp pcreposix xerces-c)
		add_custom_command(
			TARGET netbee_copy_files
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${target}> ${CMAKE_BINARY_DIR}/$<CONFIG>
			)
	endforeach()
endif()

