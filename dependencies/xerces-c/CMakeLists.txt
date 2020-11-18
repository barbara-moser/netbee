include(FetchContent)

FetchContent_Declare(
	xerces_c
	URL "${CMAKE_CURRENT_SOURCE_DIR}/xerces-c-3.2.3.zip"
	)

FetchContent_GetProperties(xerces_c)
if (NOT xerces_c_POPULATED)
	FetchContent_Populate(xerces_c)
	add_subdirectory("${xerces_c_SOURCE_DIR}" "${xerces_c_BINARY_DIR}")
	foreach(target createdocs doc-style doc-xml)
		set_target_properties(${target} PROPERTIES FOLDER "Dependencies/Xerces-C/Documentation")
	endforeach()
	foreach(target xerces-c xerces-c-headers xerces-c-missing)
		set_target_properties(${target} PROPERTIES
			FOLDER "Dependencies/Xerces-C"
			INTERFACE_INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${xerces_c_SOURCE_DIR}/src;${xerces_c_BINARY_DIR}/src>"
		)
	endforeach()
	foreach(target CreateDOMDocument DOMCount DOMPrint EnumVal MemParse PParse PSVIWriter Redirect SAX2Count SAX2Print SAXCount SAXPrint SCMPrint SEnumVal StdInParse XInclude)
		set_target_properties(${target} PROPERTIES FOLDER "Dependencies/Xerces-C/Samples")
	endforeach()
	foreach(target Char16Test DOMMemTest DOMTest DOMTraversalTest DOMTypeInfoTest EncodingTest InitTermTest MemHandlerTest NetAccessorTest Normalizer RangeTest ThreadTest XSerializerTest XSTSHarness XSValueTest)
		set_target_properties(${target} PROPERTIES FOLDER "Dependencies/Xerces-C/Tests")
	endforeach()
endif()
