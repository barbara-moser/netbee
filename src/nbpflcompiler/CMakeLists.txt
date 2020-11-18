add_library(nbpflcompiler SHARED)

include(CMakeDependentOption)

set(NETVM_COMMON_DIR ../nbnetvm/netvm_ir)
set(NETVM_OPT_DIR ../nbnetvm/jit/opt)

option(
	_DEBUG_SSA
	"turns on ssa debugging"
	OFF
	)

option(
	_DEBUG_OPTIMIZER
	"turns on optimizer debugging"
	OFF
	)

option(
	_DEBUG_REASS
	"turns on optimizer debugging"
	OFF
	)

option(
	_ENABLE_PFLC_PROFILING
	"Turns on NetPFLCompiler profiling data"
	OFF
)

option(
	ENABLE_COMPILER_PROFILING
	"Turns on printing of some informations about the compiler"
	OFF
)

option(
	ENABLE_FIELD_OPT
	"Turns off field compact-optimization"
	ON
)

CMAKE_DEPENDENT_OPTION(
	ENABLE_PFLFRONTEND_PROFILING
	"Turns on some code that measures the execution time of the NetPFL frontend"
	OFF
	"NOT ENABLE_PFLFRONTEND_PROFILING_USEC"
  	OFF
)


CMAKE_DEPENDENT_OPTION(
	ENABLE_PFLFRONTEND_PROFILING_USEC
	"Turns on some code that measures the execution time of the NetPFL frontend (used for Ivano's paper)"
	OFF
	"NOT ENABLE_PFLFRONTEND_PROFILING"
  	OFF
)

CMAKE_DEPENDENT_OPTION(
	ENABLE_PEG_AND_xpFSA_PROFILING
	"Turns on some code that counts the number of states, transitions and self loops of the PEG and the xpFSA"
	OFF
	"NOT ENABLE_PFLFRONTEND_PROFILING AND NOT ENABLE_PFLFRONTEND_PROFILING_USEC"
  	OFF
)

CMAKE_DEPENDENT_OPTION(
  ENABLE_PFLFRONTEND_DBGFILES
  "Enables the generation of DOT files and other files used for debug (forcefully DISABLED if profiling)"
  OFF
  "NOT ENABLE_PFLFRONTEND_PROFILING AND NOT ENABLE_PFLFRONTEND_PROFILING_USEC"
  OFF
)

CMAKE_DEPENDENT_OPTION(
  ENABLE_STATISTICS
  "Enables statistics about the protocols (forcefully DISABLED if profiling)"
  OFF
  "NOT ENABLE_PFLFRONTEND_PROFILING AND NOT ENABLE_PFLFRONTEND_PROFILING_USEC"
  OFF
)

target_compile_definitions(nbpflcompiler PRIVATE
	$<$<BOOL:${_DEBUG_SSA}>:_DEBUG_SSA>
	$<$<BOOL:${_DEBUG_OPTIMIZER}>:_DEBUG_OPTIMIZER>
	$<$<BOOL:${_DEBUG_REASS}>:_DEBUG_REASS>
	$<$<BOOL:${_ENABLE_PFLC_PROFILING}>:_ENABLE_PFLC_PROFILING>
	$<$<BOOL:${ENABLE_COMPILER_PROFILING}>:ENABLE_COMPILER_PROFILING>
	$<$<BOOL:${ENABLE_PFLFRONTEND_PROFILING}>:ENABLE_PFLFRONTEND_PROFILING>
	$<$<BOOL:${ENABLE_PFLFRONTEND_PROFILING_USEC}>:ENABLE_PFLFRONTEND_PROFILING_USEC>
	$<$<BOOL:${ENABLE_FIELD_OPT}>:ENABLE_FIELD_OPT>
	$<$<BOOL:${ENABLE_PEG_AND_xpFSA_PROFILING}>:ENABLE_PEG_AND_xpFSA_PROFILING>
	$<$<BOOL:${ENABLE_PFLFRONTEND_DBGFILES}>:ENABLE_PFLFRONTEND_DBGFILES>
	$<$<BOOL:${ENABLE_STATISTICS}>:ENABLE_STATISTICS>
	_PFL_BUILD
)

mark_as_advanced(
	_DEBUG_SSA _DEBUG_OPTIMIZER _ENABLE_PFLC_PROFILING _DEBUG_REASS
)

# Create grammar files
add_custom_command(
		OUTPUT
			${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp
			${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
			${CMAKE_CURRENT_BINARY_DIR}/parser.hpp
		COMMAND win_flex -i -P"pfl_" -o${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp scanner.l
		COMMAND win_bison --verbose --defines=${CMAKE_CURRENT_BINARY_DIR}/parser.hpp -p"pfl_" -o${CMAKE_CURRENT_BINARY_DIR}/parser.cpp gramm.y
		DEPENDS scanner.l gramm.y
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)

# Set source files
target_sources(nbpflcompiler PRIVATE
	../nbee/globals/globals.h
	../nbee/globals/debug.h
	../nbee/globals/debug.c
	../nbee/globals/utils.h
	../nbee/globals/utils.c
	../nbee/globals/profiling-functions.c
	../../include/nbpflcompiler.h
	../../include/nbpflcompiler_exports.h
	${NETVM_COMMON_DIR}/digraph.h
	${NETVM_COMMON_DIR}/irnode.h
	${NETVM_COMMON_DIR}/basicblock.h
	${NETVM_COMMON_DIR}/cfg.h
	adjmatrix.h
	bitmatrix.cpp
	bitmatrix.h
	${NETVM_COMMON_DIR}/bitvectorset.cpp
	${NETVM_COMMON_DIR}/bitvectorset.h
	pflcfg.cpp
	pflcfg.h
	cfg_edge_splitter.h
	${NETVM_COMMON_DIR}/cfgdom.h
	${NETVM_COMMON_DIR}/cfg_ssa.h
	${NETVM_COMMON_DIR}/cfg_ssa.cpp
	cfgvisitor.cpp
	cfgvisitor.h
	cfgwriter.cpp
	cfgwriter.h
	compile.h
	compilerconsts.cpp
	compilerconsts.h
	compunit.cpp
	compunit.h
	compfields.cpp
	compfields.h
	${NETVM_OPT_DIR}/deadcode_elimination_2.h
	${NETVM_OPT_DIR}/optimization_step.h
	defs.h
	dump.cpp
	dump.h
	errors.cpp
	errors.h
	
	filtersubgraph.cpp
	filtersubgraph.h
	
	encapgraph.cpp
	encapgraph.h

	encapfsa.cpp
	encapfsa.h
	
	globalinfo.cpp
	globalinfo.h
	globalsymbols.cpp
	globalsymbols.h
	gramm.y
	hashtable.h
	intsymboltable.h
	ircodegen.cpp
	ircodegen.h
	irlowering.cpp
	irlowering.h
	irnoderegrenamer.cpp
	irnoderegrenamer.h
	iroperators.h
	irops.h
	netilopcodes.h
	netiltracebuilder.cpp
	netiltracebuilder.h
	nbpflcompiler.cpp
	netpflfrontend.cpp
	netpflfrontend.h
	node_translation.h
	node_translation.cpp
	${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
	${CMAKE_CURRENT_BINARY_DIR}/parser.hpp
	pdlparser.cpp
	pdlparser.h
	pflexpression.cpp
	pflexpression.h
	mironode.h
	mironode.cpp
	protographshaper.cpp
	protographshaper.h
	protographwriter.cpp
	protographwriter.h
	reassociation_fixer.cpp
	reassociation_fixer.h
	${NETVM_COMMON_DIR}/registers.h
	${NETVM_COMMON_DIR}/registers.cpp
	${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp
	scanner.l
	statements.cpp
	statements.h
	strsymboltable.h
	strutils.h
	symbols.h
	symbols.cpp
	symtabletree.cpp
	symtabletree.h
	${NETVM_COMMON_DIR}/taggable.h
	${NETVM_COMMON_DIR}/taggable.cpp
	pfl_trace_builder.h
	pfl_trace_builder.cpp
	tree.cpp
	tree.h
	typecheck.cpp
	typecheck.h
	unistd.h
	utils.cpp
	utils.h
	visit.cpp
	visit.h
	
#SFT files:
	./sft/sft_defs.h
	./sft/sft_traits.hpp
	./sft/sft_basic_iterator.hpp 
	./sft/sft_generic_set.hpp    
	./sft/sft.hpp
	./sft/sft_writer.hpp
	./sft/sft_tools.hpp
	./sft/digraph_stlplus/containers_fixes.hpp 
	./sft/digraph_stlplus/digraph.tpp
	./sft/digraph_stlplus/exceptions.hpp
	./sft/digraph_stlplus/safe_iterator.hpp
	./sft/digraph_stlplus/digraph.hpp
	./sft/digraph_stlplus/exceptions.cpp
	./sft/digraph_stlplus/safe_iterator.tpp
)


# MACOSX
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message(STATUS "Mac OS X detected!")
	target_compile_options(nbpflcompiler PRIVATE -fno-common)
endif()

# Default directories for include files
target_include_directories(nbpflcompiler PUBLIC
#Note OM: please leave the src directory in the default include directories, otherwise the fake unistd.h file required by scanner.cpp will not be found on Win32
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/sft
	${CMAKE_CURRENT_SOURCE_DIR}/sft/digraph_stlplus
	${CMAKE_CURRENT_SOURCE_DIR}/../../include
	${CMAKE_CURRENT_SOURCE_DIR}/../nbnetvm/netvm_ir
	${NETVM_OPT_DIR}
)

# Platform-specific definitions
if(WIN32)
	target_compile_definitions(nbpflcompiler PRIVATE
		_CRT_SECURE_NO_WARNINGS
		_CRT_SECURE_NO_DEPRECATE
		WIN32_LEAN_AND_MEAN
		PFLCOMPILER_EXPORTS
	)
endif()

target_link_libraries(nbpflcompiler PUBLIC netbeeOptions nbprotodb pcre)

# Set additional flags that are compiler-dependent
if(WIN32)
	set_target_properties(
		nbpflcompiler
		PROPERTIES
		COMPILE_FLAGS "/Zc:wchar_t-"
	)
endif()
