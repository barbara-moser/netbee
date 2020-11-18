add_library(netbeeOptions INTERFACE)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
	target_compile_options(netbeeOptions INTERFACE -Wno-write-strings)
endif()

##### Paper testing (e.g., pFSA - ToN)
option(
  ENABLE_TEST_REPORTS
  "Enable various test reports for paper testing (e.g., pFSA - ToN)"
  OFF
)

##### Ivano's paper testing
option(
  ENABLE_PFLFRONTEND_PROFILING_USEC
  "Enable few test reports for Ivano's paper testing"
  OFF
)

## Code profiling
option(
  ENABLE_PROFILING
  "Enable profiling symbols generation by the C++ compiler. This option DOES NOT enable any NetBee-specific profiling code and WILL impact on performance."
  OFF
)

set_target_properties(netbeeOptions PROPERTIES
	INTERFACE_COMPILE_DEFINITIONS
		"$<$<BOOL:${ENABLE_TEST_REPORTS}>:ENABLE_TEST_REPORTS>;$<$<BOOL:${ENABLE_PFLFRONTEND_PROFILING_USEC}>:ENABLE_PFLFRONTEND_PROFILING_USEC>"
	INTERFACE_COMPILE_OPTIONS
		$<$<BOOL:${ENABLE_PROFILING}>:-pg>
	INTERFACE_LINK_OPTIONS
		$<$<BOOL:${ENABLE_PROFILING}>:$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:-pg>>
)

add_subdirectory(nbnetvm)
add_subdirectory(nbprotodb)
add_subdirectory(nbsockutils)
add_subdirectory(nbpflcompiler)
add_subdirectory(nbee)
