add_library(nbsockutils SHARED)

# Set source files
target_sources(nbsockutils PRIVATE
	../nbee/globals/debug.h
	../nbee/globals/debug.c
	../nbee/globals/utils.h
	../nbee/globals/utils.c
	../../include/nbsockutils.h
	sockutils.c
)

target_include_directories(nbsockutils PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../../include
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Platform-specific definitions
if(WIN32)
	target_compile_definitions(nbsockutils PRIVATE
		_CRT_SECURE_NO_WARNINGS
		_CRT_SECURE_NO_DEPRECATE
		WIN32_LEAN_AND_MEAN
		SOCKUTILS_EXPORTS
	)
endif()

target_link_libraries(nbsockutils PRIVATE
	$<$<PLATFORM_ID:Windows>:ws2_32.lib>
	netbeeOptions
)
