add_executable(netvmburg)

set_target_properties(netvmburg PROPERTIES
	FOLDER "nbnetvm/tools"
)

# Set source files
target_sources(netvmburg PRIVATE
	netvmburg.cpp
	netvmburg.h
	"${CMAKE_CURRENT_BINARY_DIR}/parser.cpp"
)

target_include_directories(netvmburg PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Platform-specific definitions
IF(WIN32)
	target_compile_definitions(netvmburg PRIVATE
		_CRT_SECURE_NO_WARNINGS
		_CRT_SECURE_NO_DEPRECATE
		_SCL_SECURE_NO_DEPRECATE
#		_CRTDBG_MAP_ALLOC
		WIN32_LEAN_AND_MEAN
    __STDC__
	)
ENDIF(WIN32)

# Create parser through bison
ADD_CUSTOM_COMMAND(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
	COMMAND win_bison
	ARGS -y ${CMAKE_CURRENT_SOURCE_DIR}/netvmburg.y -o "${CMAKE_CURRENT_BINARY_DIR}/parser.cpp"
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/netvmburg.y
)
