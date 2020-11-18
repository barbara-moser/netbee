include(FetchContent)

FetchContent_Declare(
	pcre8_44
	URL "${CMAKE_CURRENT_SOURCE_DIR}/pcre-8.44.zip"
	)

FetchContent_GetProperties(pcre8_44)
if (NOT pcre8_44_POPULATED)
	FetchContent_Populate(pcre8_44)
	add_subdirectory("${pcre8_44_SOURCE_DIR}" "${pcre8_44_BINARY_DIR}")
endif()
foreach(target pcre pcre_scanner_unittest pcre_stringpiece_unittest pcrecpp pcrecpp_unittest pcregrep pcreposix pcretest)
	set_target_properties(${target} PROPERTIES FOLDER "Dependencies/PCRE" INTERFACE_INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${pcre8_44_BINARY_DIR}>")
endforeach()
