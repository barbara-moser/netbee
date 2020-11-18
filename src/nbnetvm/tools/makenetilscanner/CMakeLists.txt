add_executable(makenetilscanner)

set_target_properties(makenetilscanner PROPERTIES
	FOLDER "nbnetvm/tools"
)

target_sources(makenetilscanner PRIVATE
  makenetilscanner.c
)

target_include_directories(makenetilscanner PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/../../include"
)
 
# Platform-specific definitions
IF(WIN32)
	target_compile_definitions(makenetilscanner PRIVATE
		_CRT_SECURE_NO_WARNINGS
		_CRT_SECURE_NO_DEPRECATE
		_SCL_SECURE_NO_DEPRECATE
		WIN32_LEAN_AND_MEAN
    __STDC__
	)
ENDIF(WIN32)
