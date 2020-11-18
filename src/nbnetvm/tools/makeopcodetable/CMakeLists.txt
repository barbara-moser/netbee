add_executable(makeopcodetable)

set_target_properties(makeopcodetable PROPERTIES
	FOLDER "nbnetvm/tools"
)

# Set source files
target_sources(makeopcodetable PRIVATE
  makeopcodetable.c
)


target_include_directories(makeopcodetable PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/../../include"
)
 
# Platform-specific definitions
IF(WIN32)
	target_compile_definitions(makeopcodetable PRIVATE
		_CRT_SECURE_NO_WARNINGS
		_CRT_SECURE_NO_DEPRECATE
		_SCL_SECURE_NO_DEPRECATE
#		_CRTDBG_MAP_ALLOC
		WIN32_LEAN_AND_MEAN
    __STDC__
	)
ENDIF(WIN32)
