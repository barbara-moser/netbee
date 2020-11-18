add_library(nbprotodb SHARED)

add_custom_command(
	OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/expr-grammar.tab.c
	${CMAKE_CURRENT_BINARY_DIR}/expr-grammar.tab.h
	${CMAKE_CURRENT_BINARY_DIR}/expr-scanner.c
	COMMAND win_bison -p"pdl_" --defines=${CMAKE_CURRENT_BINARY_DIR}/expr-grammar.tab.h -o ${CMAKE_CURRENT_BINARY_DIR}/expr-grammar.tab.c expr-grammar.y
	# bison -t: run in debug mode
	COMMAND win_flex -P"pdl_" -o${CMAKE_CURRENT_BINARY_DIR}/expr-scanner.c expr-scanner.l
	# flex -d: run in debug mode
	# This command is required on MacOSX; it is not requiried in Windows
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
# Set source files
target_sources(nbprotodb PRIVATE
	../nbee/globals/globals.h
	../nbee/globals/debug.h
	../nbee/globals/debug.c
	../nbee/globals/utils.h
	../nbee/globals/utils.c
	../nbee/utils/netpdlutils.h
	../nbee/utils/netpdlutils.c

	../../include/nbprotodb.h
	../../include/nbprotodb_defs.h
	../../include/nbprotodb_exports.h
	../../include/nbprotodb_elements_xml.h

	elements_create.h
	elements_create.cpp
	elements_delete.h
	elements_delete.cpp
	elements_organize.h
	elements_organize.cpp
	elements_serialize.h
	elements_serialize.cpp
	lists_organize.h
	lists_organize.cpp
	expressions.h
	expressions.cpp
	expr-scanner.l
	expr-grammar.y
	${CMAKE_CURRENT_BINARY_DIR}/expr-scanner.c
	${CMAKE_CURRENT_BINARY_DIR}/expr-grammar.tab.h
	${CMAKE_CURRENT_BINARY_DIR}/expr-grammar.tab.c
	protodb_globals.h
	protodb.cpp
	sax_handler.h
	sax_handler.cpp
	sax_parser.h
	sax_parser.cpp
)

# MACOSX
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message (STATUS "Mac OS X detected!")
	set (CMAKE_C_FLAGS "-fno-common")
	set (CMAKE_CXX_FLAGS "-fno-common")
endif()

# Default directories for include files
target_include_directories(nbprotodb PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../../include
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(nbprotodb PUBLIC
	netbeeOptions
	xerces-c
	pcre
)

# Platform-specific definitions
if(WIN32)
	target_compile_definitions(nbprotodb PRIVATE
		-D_CRT_SECURE_NO_WARNINGS
		-D_CRT_SECURE_NO_DEPRECATE
		-D_CRT_NONSTDC_NO_DEPRECATE 
		-D_SCL_SECURE_NO_DEPRECATE
		-DWIN32_LEAN_AND_MEAN
		-DYY_NO_UNISTD_H
		-DPROTODB_EXPORTS
	)
endif(WIN32)
