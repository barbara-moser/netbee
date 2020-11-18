add_library(nbee SHARED)

target_sources(nbee PRIVATE
	../../include/nbee.h
	../../include/nbee_initcleanup.h
	../../include/nbee_netpdlutils.h
	../../include/nbee_packetdecoder.h
	../../include/nbee_packetdecoderutils.h
	../../include/nbee_packetdumpfiles.h
	../../include/nbee_pxmlreader.h
	../../include/nbee_packetengine.h
	../../include/nbee_extractedfieldreader.h
	../../include/nbee_profiler.h

	globals/globals.h
	globals/debug.h
	globals/debug.c
	globals/utils.h
	globals/utils.c
	globals/profiling.h
	globals/profiling.cpp
	globals/profiling-functions.h
	globals/profiling-functions.c
	globals/pcap_structures.h

	decoder/netpdldecoder.h
	decoder/netpdldecoder.cpp
	decoder/netpdldecoderutils.h
	decoder/netpdldecoderutils.cpp
	decoder/netpdlexpression.h
	decoder/netpdlexpression.cpp
	decoder/netpdllookuptables.h
	decoder/netpdllookuptables.cpp
	decoder/netpdlprotodecoder.h
	decoder/netpdlprotodecoder.cpp
	decoder/netpdlstandardvars.h
	decoder/netpdlstandardvars.cpp
	decoder/netpdlvariables.h
	decoder/netpdlvariables.cpp
	decoder/pdmlmaker.h
	decoder/pdmlmaker.cpp
	decoder/pdmlreader.h
	decoder/pdmlreader.cpp
	decoder/psmlmaker.h
	decoder/psmlmaker.cpp
	decoder/psmlreader.h
	decoder/psmlreader.cpp
	decoder/pxmlreader.h
	decoder/pxmlreader.cpp

	decoder/decoderplugin/decoder_plugin.h
	decoder/decoderplugin/decoder_plugin.cpp
	decoder/decoderplugin/decoder_dnsdomainname.cpp

	decoder/showplugin/show_plugin.h
	decoder/showplugin/show_plugin.cpp
	decoder/showplugin/show_ip46name.cpp
	decoder/showplugin/show_dnsdomainname.cpp
	decoder/showplugin/show_netbiosname.cpp
	decoder/showplugin/native_showfunctions.h
	decoder/showplugin/native_showfunctions.cpp

	misc/os_utils.h
	misc/os_utils.cpp
	misc/exports.cpp
	misc/initialize.h
	misc/initialize.cpp
	misc/update_netpdlfile.cpp

	nbpacketengine/nbpacketengine.cpp
	nbpacketengine/fieldreader.cpp
	nbpacketengine/nbeepacketengine.h
	nbpacketengine/nbeefieldreader.h
	
	packetprocessing/packetdumpfile.h
	packetprocessing/packetdumpfile.cpp
	packetprocessing/packet_pcapdumpfile.h
	packetprocessing/packet_pcapdumpfile.cpp

	utils/asciibuffer.h
	utils/asciibuffer.cpp
	utils/netpdlutils.h
	utils/netpdlutils.c
)

find_package(PCAP)

# Default directories for include files
target_include_directories(nbee PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../../include
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(nbee PUBLIC
	xerces-c
	pcre
	$<$<BOOL:${PCAP_FOUND}>:pcap>
	netbeeOptions
	nbprotodb
	nbpflcompiler
	nbsockutils
	nbnetvm
	$<$<AND:$<PLATFORM_ID:Windows>,$<BOOL:${PCAP_FOUND}>>:packet>
	$<$<PLATFORM_ID:Linux>:${CMAKE_DL_LIBS}>
)

target_compile_definitions(nbee PRIVATE $<$<BOOL:${PCAP_FOUND}>:HAVE_PCAP>)

# Platform-specific definitions
if(WIN32)
	target_compile_definitions(nbee PRIVATE
		_CRT_SECURE_NO_WARNINGS
		_CRT_SECURE_NO_DEPRECATE
		_CRT_NONSTDC_NO_DEPRECATE 
		WIN32_LEAN_AND_MEAN
		NBEE_EXPORTS
		HAVE_REMOTE
	)
endif()
