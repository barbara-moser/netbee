# definition of helpful directories
set(NETVM_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(NETVM_SRC_DIR ${NETVM_BASE_DIR})
set(NETVM_COMMON_DIR ${NETVM_BASE_DIR}/netvm_ir)
set(NETVM_ASM_DIR ${NETVM_SRC_DIR}/assembler)
set(NETVM_JIT_DIR ${NETVM_SRC_DIR}/jit)

add_library(
	nbnetvm
	SHARED
)

set_target_properties(nbnetvm PROPERTIES
	FOLDER "nbnetvm"
)

find_package(PCAP)

target_compile_definitions(nbnetvm PRIVATE USE_JIT _JIT_BUILD $<$<BOOL:${PCAP_FOUND}>:HAVE_PCAP>)


# MACOSX
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	target_compile_options(nbnetvm PRIVATE
		"-fno-common"
	)
	target_compile_definitions(nbnetvm PRIVATE
		__APPLE__
	)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

target_include_directories(nbnetvm
	PRIVATE
		assembler
		../../include
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}
		netvm_ir
		${NETVM_JIT_DIR}
)

if(WIN32)
	include_directories(${NETVM_BASE_DIR}/../../../WPdPack/Include)
	link_directories(${NETVM_BASE_DIR}/../../../WPdPack/Lib)
endif()


###################################################################################
#
# OPTIONS for compilation

# Choose enabled backend
option(
	ENABLE_X86_BACKEND
	"Turn on the Intel x86 backend (32 bits) for NetVM"
	OFF
)
option(
	ENABLE_X64_BACKEND
	"Turn on the Intel x64 backend (64 bits) for NetVM"
	OFF
)
option(
	ENABLE_X11_BACKEND
	"Turn on the Xelerated x11 backend for NetVM"
	OFF
)
option(
	ENABLE_OCTEON_BACKEND
	"Turn on the Cavium Octeon backend for NetVM"
	OFF
)
option(
	ENABLE_OCTEONC_BACKEND
	"Turn on the Cavium Octeon backend for NetVM in C"
	OFF
)

# Choose profiling options
option(
	ENABLE_COMPILER_PROFILING
	"Turn on printing of some informations about the compiler"
	OFF
)

option(
	ENABLE_CODE_PROFILING
	"Turn on the code that allows to measure the execution time of the NetVM code (jit or native code)"
	OFF
)

option(
	ENABLE_COUNTERS_PROFILING
	"Turn on COUNTERS PROFILING"
	OFF
)

option(
	ENABLE_PROFILING_COUNTERS
	"Enable RTE profiling counters"
	OFF
)

option(ENABLE_JIT_PROFILING
	"Enable profiling of the jit"
	OFF
)

option(ENABLE_CODEVERIFY_PROFILING
	"Enable profiling of the code verification (in the interpreter)"
	# Please remember that this code works only when the interpreter is used
	OFF
)

# Enabling logging
option(
	ENABLE_INTERNAL_LOGGING
	"Turn on some additional logging (e.g., dump of the executed netil code) on file for debuging purposes"
	OFF
)


if(ENABLE_COMPILER_PROFILING)
	target_compile_definitions(nbnetvm PRIVATE ENABLE_COMPILER_PROFILING ENABLE_PROFILING)
endif(ENABLE_COMPILER_PROFILING)

if(ENABLE_CODE_PROFILING)
	target_compile_definitions(nbnetvm PRIVATE CODE_PROFILING ENABLE_PROFILING)
endif(ENABLE_CODE_PROFILING)

if(ENABLE_COUNTERS_PROFILING)
	target_compile_definitions(nbnetvm PRIVATE CODE_PROFILING COUNTERS_PROFILING ENABLE_PROFILING)
endif(ENABLE_COUNTERS_PROFILING)

if(ENABLE_PROFILING_COUNTERS)
	target_compile_definitions(nbnetvm PRIVATE RTE_PROFILE_COUNTERS ENABLE_PROFILING)
endif(ENABLE_PROFILING_COUNTERS)

if(ENABLE_DYNAMIC_PROFILING)
	target_compile_definitions(nbnetvm PRIVATE RTE_DYNAMIC_PROFILE ENABLE_PROFILING)
endif(ENABLE_DYNAMIC_PROFILING)

if(ENABLE_JIT_PROFILING)
	target_compile_definitions(nbnetvm PRIVATE JIT_RTE_PROFILE_COUNTERS ENABLE_PROFILING)
endif(ENABLE_JIT_PROFILING)

if(ENABLE_CODEVERIFY_PROFILING)
	target_compile_definitions(nbnetvm PRIVATE CODEVERIFY_PROFILING ENABLE_PROFILING)
endif(ENABLE_CODEVERIFY_PROFILING)

if(ENABLE_INTERNAL_LOGGING)
	target_compile_definitions(nbnetvm PRIVATE ENABLE_NETVM_LOGGING)
endif(ENABLE_INTERNAL_LOGGING)

# Choose debug options
option(
	_DEBUG_JIT_IF
	"Turns on jit interface debugging"
	OFF
)

option(
	_DEBUG_CFG_BUILD
	"Turns on cfg building debugging"
	OFF
)

option(
	_DEBUG_SSA
	"turns on ssa debugging"
	OFF
)

option(
	_DEBUG_BCHECK
	"turns on bcheck remove debugging"
	OFF
)

option(
	_DEBUG_SWITCH_LOWERING
	"turns on switch lowering debugging"
	OFF
)

option(
	_DEBUG_LIVENESS
	"turns on livness debugging"
	OFF
)

option(
	_DEBUG_REG_ALLOC
	"turns on register allocation debugging"
	OFF
)

option(
	_DEBUG_OPTIMIZER
	"turns on optimizer debugging"
	OFF
)

option(
	_DEBUG_ALGEBRAIC_SIMPLIFICATION
	"turns on algebraic simplification debugging"
	OFF
)

option(
	_DEBUG_DEAD_CODE_ELIMINATION
	"turns on dead code elimination debugging"
	OFF
)

option(
	_DEBUG_CONTROL_FLOW_SIMPLIFICATION	
	"turns on control flow simplification debugging"
	OFF
)

option(
	_DEBUG_REDISTRIBUTION
	"turns on redistribution debugging"
	OFF
)

option(
	_DEBUG_CONSTANT_PROPAGATION
	"turns on constant propagation debugging"
	OFF
)

option(
	_DEBUG_REASS
	"turns on optimizer debugging"
	OFF
)

option(
	_DEBUG_SPILL
	"Turns on Spill debugging"
	OFF
)

option(
	_DEBUG_COMMON_BACKEND
	"Turns on the common backend debug"
	OFF
)

option(
	_DEBUG_X64_BACKEND
	"Turns on x64 backend debug"
	OFF
)

option(
	_DEBUG_OPTIMIZATION_ALGORITHMS
	"Turns on the optimization algorithms debug"
	OFF
)

target_compile_definitions(nbnetvm PRIVATE
	$<$<BOOL:${_DEBUG_JIT_IF}>:_DEBUG_JIT_IF>
	$<$<BOOL:${_DEBUG_CFG_BUILD}>:_DEBUG_CFG_BUILD>
	$<$<BOOL:${_DEBUG_SSA}>:_DEBUG_SSA>
	$<$<BOOL:${_DEBUG_BCHECK}>:_DEBUG_BCHECK>
	$<$<BOOL:${_DEBUG_SWITCH_LOWERING}>:_DEBUG_SWITCH_LOWERING>
	$<$<BOOL:${_DEBUG_LIVENESS}>:_DEBUG_LIVENESS>
	$<$<BOOL:${_DEBUG_REG_ALLOC}>:_DEBUG_REG_ALLOC>
	$<$<BOOL:${_DEBUG_OPTIMIZER}>:_DEBUG_OPTIMIZER>
	$<$<BOOL:${_DEBUG_ALGEBRAIC_SIMPLIFICATION}>:_DEBUG_ALGEBRAIC_SIMPLIFICATION>
	$<$<BOOL:${_DEBUG_DEAD_CODE_ELIMINATION}>:_DEBUG_DEAD_CODE_ELIMINATION>
	$<$<BOOL:${_DEBUG_CONTROL_FLOW_SIMPLIFICATION}>:_DEBUG_CONTROL_FLOW_SIMPLIFICATION>
	$<$<BOOL:${_DEBUG_REDISTRIBUTION}>:_DEBUG_REDISTRIBUTION>
	$<$<BOOL:${_DEBUG_CONSTANT_PROPAGATION}>:_DEBUG_CONSTANT_PROPAGATION>
	$<$<BOOL:${_DEBUG_REASS}>:_DEBUG_REASS>
	$<$<BOOL:${_DEBUG_SPILL}>:_DEBUG_SPILL>
	$<$<BOOL:${_DEBUG_COMMON_BACKEND}>:_DEBUG_COMMON_BACKEND>
	$<$<BOOL:${_DEBUG_X64_BACKEND}>:_DEBUG_X64_BACKEND>
	$<$<BOOL:${_DEBUG_OPTIMIZATION_ALGORITHMS}>:_DEBUG_OPTIMIZATION_ALGORITHMS>
)

mark_as_advanced(
	_DEBUG_CFG_BUILD 
	_DEBUG_SPILL 
	_DEBUG_REG_ALLOC 
	_DEBUG_LIVENESS 
	_DEBUG_SSA 
	_DEBUG_BCHECK 
	_DEBUG_OPTIMIZER 
	_DEBUG_ALGEBRAIC_SIMPLIFICATION
	_DEBUG_CONSTANT_PROPAGATION
	_DEBUG_DEAD_CODE_ELIMINATION
	_DEBUG_CONTROL_FLOW_SIMPLIFICATION
	_DEBUG_REDISTRIBUTION
	_DEBUG_JIT_IF
	_DEBUG_REASS
	_DEBUG_SWITCH_LOWERING
)

add_subdirectory(${NETVM_BASE_DIR}/tools/makenetilscanner)
add_subdirectory(${NETVM_BASE_DIR}/tools/makeopcodetable)
add_subdirectory(${NETVM_BASE_DIR}/tools/netvmburg)

set(scanner_prefix nvmparser_)

###############################################################################
#
# code generation
#

add_custom_command(
	OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/opcodestable.txt
	COMMAND makeopcodetable "../../opcodes.txt" 1> "${CMAKE_CURRENT_BINARY_DIR}/opcodestable.txt"
	DEPENDS
	${NETVM_SRC_DIR}/opcodes.txt makeopcodetable
	WORKING_DIRECTORY "${NETVM_BASE_DIR}/tools/bin"
	)

add_custom_command(
	OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/nvm_gramm.tab.c
	${CMAKE_CURRENT_BINARY_DIR}/nvm_gramm.tab.h
	COMMAND win_bison -bgramm --defines=${CMAKE_CURRENT_BINARY_DIR}/nvm_gramm.tab.h -p "${scanner_prefix}" "../../assembler/nvm_gramm.y" -o "${CMAKE_CURRENT_BINARY_DIR}/nvm_gramm.tab.c"
	DEPENDS ${NETVM_ASM_DIR}/nvm_gramm.y
	WORKING_DIRECTORY "${NETVM_BASE_DIR}/tools/bin"
	)

add_custom_command(
	OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/scanner.l
	${CMAKE_CURRENT_BINARY_DIR}/scanner.c

	COMMAND makenetilscanner "../../opcodes.txt" "../../assembler/scanner-template.l" > "${CMAKE_CURRENT_BINARY_DIR}/scanner.l"

	COMMAND win_flex -P${scanner_prefix} -i -o"${CMAKE_CURRENT_BINARY_DIR}/scanner.c" "${CMAKE_CURRENT_BINARY_DIR}/scanner.l"
	DEPENDS makenetilscanner
	${NETVM_ASM_DIR}/scanner-template.l
	${NETVM_SRC_DIR}/opcodes.txt
	WORKING_DIRECTORY "${NETVM_BASE_DIR}/tools/bin"
	)

#####################################################################################
#
# Coprocessors
#

# 3 for lookup, one for regex, one for string matching
set(AVAILABLE_COPROS_NO 5)

####################################################################################
#
# String matching
#

target_sources(nbnetvm PRIVATE
	${NETVM_SRC_DIR}/arch/generic/coprocessors/stringmatching.c
	${NETVM_SRC_DIR}/arch/generic/coprocessors/acsmx2.c
	${NETVM_SRC_DIR}/arch/generic/coprocessors/acsmx2.h
)

########################################################################
#
# Regexp
#

target_sources(nbnetvm PRIVATE
	${NETVM_SRC_DIR}/arch/generic/coprocessors/regexp.c
)

#################################################################################
#
# files for all runtime backend
#

target_sources(nbnetvm PRIVATE
	${NETVM_SRC_DIR}/assembler/dump.c
	${CMAKE_CURRENT_BINARY_DIR}/scanner.c
	${CMAKE_CURRENT_BINARY_DIR}/nvm_gramm.tab.c
	${NETVM_SRC_DIR}/assembler/netil_assembler.c
	${NETVM_SRC_DIR}/assembler/hashtable.c
	${NETVM_SRC_DIR}/helpers.c
	${NETVM_SRC_DIR}/rt_environment.c
	${NETVM_SRC_DIR}/utils/slinkedlst.c
	${NETVM_SRC_DIR}/utils/dlinkedlst.c
	${NETVM_SRC_DIR}/utils/hashtbl.c
	${NETVM_SRC_DIR}/utils/hashtblgen.c
	${NETVM_SRC_DIR}/vm_application.c
	${NETVM_SRC_DIR}/coprocessor.c
	${NETVM_SRC_DIR}/arch/generic/coprocessors/lookup.c
	${NETVM_SRC_DIR}/arch/generic/coprocessors/lookup-new.c
	${NETVM_SRC_DIR}/arch/generic/coprocessors/lookup_ex.c
	${NETVM_ASM_DIR}/nvm_gramm.y
	${NETVM_ASM_DIR}/scanner-template.l
# The following is dynamically generated from the scanner-template.l above
#	${CMAKE_CURRENT_BINARY_DIR}/scanner.l
# Files for profiling
	${NETVM_SRC_DIR}/netvmprofiling.cpp
	${NETVM_SRC_DIR}/../nbee/globals/profiling-functions.c
	${NETVM_SRC_DIR}/../nbee/globals/profiling.cpp
# Files for debug and misc
	${NETVM_SRC_DIR}/../nbee/globals/debug.c
	${NETVM_SRC_DIR}/../nbee/globals/utils.c
#Opcode tables
	${NETVM_SRC_DIR}/opcodes.txt
	${NETVM_SRC_DIR}/newopcodes.txt
	${CMAKE_CURRENT_BINARY_DIR}/opcodestable.txt
	)

target_sources(nbnetvm PRIVATE
	${NETVM_SRC_DIR}/../../include/nbnetvm.h
	${NETVM_SRC_DIR}/helpers.h
	${NETVM_SRC_DIR}/netvm_bytecode.h
	${NETVM_SRC_DIR}/rt_environment.h
	${NETVM_SRC_DIR}/coprocessor.h
	${NETVM_SRC_DIR}/int_structs.h
	${NETVM_SRC_DIR}/utils/lists.h
	${NETVM_SRC_DIR}/utils/hashtbl.h
	${NETVM_SRC_DIR}/netvm_ir/bitmatrix.h
	${NETVM_SRC_DIR}/assembler/hashtable.h
	${NETVM_SRC_DIR}/assembler/netvm_dump.h
# Files for profiling
	${NETVM_SRC_DIR}/netvmprofiling.h
	${NETVM_BASE_DIR}/../nbee/globals/profiling.h
	${NETVM_BASE_DIR}/../nbee/globals/profiling-functions.h
# Files for debug
	${NETVM_BASE_DIR}/../nbee/globals/debug.h
	)

#################################################################################
#
# files for generic runtime backend
#

target_sources(nbnetvm PRIVATE
	${NETVM_SRC_DIR}/arch/generic/generic_runtime.c
	${NETVM_SRC_DIR}/arch/generic/generic_interpreter.c
	${NETVM_SRC_DIR}/arch/generic/coprocessors/coprocessors_main.c
	${NETVM_JIT_DIR}/bytecode_analyse.cpp
	${NETVM_JIT_DIR}/bytecode_segments.cpp
	${NETVM_JIT_DIR}/comperror.cpp
	${NETVM_JIT_DIR}/codetable.c
	${NETVM_JIT_DIR}/jit_interface.cpp
	${NETVM_JIT_DIR}/iltranslator.cpp
	${NETVM_JIT_DIR}/mem_translator.cpp
	${NETVM_JIT_DIR}/op_size.cpp
	${NETVM_COMMON_DIR}/bitvectorset.cpp
	${NETVM_JIT_DIR}/cfg_edge_splitter.cpp
	${NETVM_JIT_DIR}/genericbackend.cpp
	${NETVM_COMMON_DIR}/cfg_ssa.cpp
	${NETVM_JIT_DIR}/mirnode.cpp
	${NETVM_COMMON_DIR}/registers.cpp
	${NETVM_JIT_DIR}/copy_folding.cpp
	${NETVM_JIT_DIR}/opt/reassociation.cpp
	${NETVM_COMMON_DIR}/taggable.cpp
	${NETVM_JIT_DIR}/application.cpp
	${NETVM_JIT_DIR}/switch_lowering.cpp
	${NETVM_JIT_DIR}/opt/constant_folding.cpp
	${NETVM_JIT_DIR}/opt/constant_propagation.cpp
	${NETVM_JIT_DIR}/opt/controlflow_simplification.cpp
	${NETVM_JIT_DIR}/opt/redistribution.cpp
	${NETVM_JIT_DIR}/opt/copy_propagation.cpp
	${NETVM_JIT_DIR}/opt/optimizer_statistics.cpp
)

target_sources(nbnetvm PRIVATE
	${NETVM_SRC_DIR}/arch/generic/arch.h
	${NETVM_SRC_DIR}/arch/generic/generic_runtime.h
	${NETVM_SRC_DIR}/arch/generic/generic_interpreter.h
	${NETVM_COMMON_DIR}/digraph.h
	${NETVM_COMMON_DIR}/basicblock.h
	${NETVM_COMMON_DIR}/bitvectorset.h
	${NETVM_JIT_DIR}/bytecode_analyse.h
	${NETVM_JIT_DIR}/bytecode_segments.h
	${NETVM_JIT_DIR}/cfg_builder.h
	${NETVM_JIT_DIR}/cfg_loop_analyzer.h
	${NETVM_JIT_DIR}/cfg_bytecode_builder.h
	${NETVM_COMMON_DIR}/cfgdom.h
	${NETVM_COMMON_DIR}/cfg.h
	${NETVM_COMMON_DIR}/cfg_liveness.h
	${NETVM_JIT_DIR}/cfg_printer.h
	${NETVM_COMMON_DIR}/cfg_ssa.h
	${NETVM_JIT_DIR}/codetable.h
	${NETVM_JIT_DIR}/comperror.h
	${NETVM_JIT_DIR}/iltranslator.h
	${NETVM_COMMON_DIR}/irnode.h
	${NETVM_JIT_DIR}/jit_interface.h
	${NETVM_JIT_DIR}/jit_internals.h
	${NETVM_JIT_DIR}/mem_translator.h
	${NETVM_JIT_DIR}/mirnode.h
	${NETVM_JIT_DIR}/netvmjitglobals.h
	${NETVM_JIT_DIR}/visitors.h
	${NETVM_JIT_DIR}/genericbackend.h
	${NETVM_COMMON_DIR}/register_mapping.h
	${NETVM_JIT_DIR}/op_size.h
	${NETVM_JIT_DIR}/copy_folding.h
	${NETVM_JIT_DIR}/cfg_copy.h
	${NETVM_JIT_DIR}/insselector.h
	${NETVM_COMMON_DIR}/registers.h
	${NETVM_JIT_DIR}/cfg_edge_splitter.h
	${NETVM_JIT_DIR}/offsets.h
	${NETVM_COMMON_DIR}/taggable.h
	${NETVM_JIT_DIR}/application.h
	${NETVM_COMMON_DIR}/ssa_graph.h
	${NETVM_JIT_DIR}/gc_regalloc.h
	${NETVM_JIT_DIR}/switch_lowering.h
	${NETVM_JIT_DIR}/opt/deadcode_elimination_2.h
	${NETVM_JIT_DIR}/opt/constant_folding.h
	${NETVM_JIT_DIR}/opt/nvm_optimizer.h
	${NETVM_JIT_DIR}/opt/optimization_step.h
	${NETVM_JIT_DIR}/opt/constant_propagation.h
	${NETVM_JIT_DIR}/opt/deadcode_elimination.h
	${NETVM_JIT_DIR}/opt/controlflow_simplification.h
	${NETVM_JIT_DIR}/opt/redistribution.h
	${NETVM_JIT_DIR}/opt/reassociation.h
	${NETVM_COMMON_DIR}/tracebuilder.h
	${NETVM_JIT_DIR}/opt/copy_propagation.h
	${NETVM_JIT_DIR}/opt/optimizer_statistics.h
	${NETVM_JIT_DIR}/opt/bcheck_remove.h
	${NETVM_ASM_DIR}/compiler.h
)

###########################################################################################
#
# file for compilers backends with generic runtime
#

#
# IA32
#

if(ENABLE_X86_BACKEND)
	target_compile_definitions(nbnetvm PRIVATE ENABLE_X86_BACKEND)
	set(X86DIS_SRC_DIR ${NETVM_JIT_DIR}/x86_x64_disasm )
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ia32/inssel-ia32.cpp
		${CMAKE_CURRENT_BINARY_DIR}/ia32/inssel-ia32.h
		COMMAND ${CMAKE__COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/ia32
		COMMAND netvmburg
		ARGS
		#comment the following line to disable constant swapping
		-DSWAP_CONST
		-C IA32InsSelector -p -d ${CMAKE_CURRENT_BINARY_DIR}/ia32/inssel-ia32.h ../../jit/ia32/inssel-ia32.brg > ${CMAKE_CURRENT_BINARY_DIR}/ia32/inssel-ia32.cpp
		DEPENDS netvmburg
		${NETVM_JIT_DIR}/ia32/inssel-ia32.brg
		WORKING_DIRECTORY "${NETVM_BASE_DIR}/tools/bin"
		)

	target_sources(nbnetvm PRIVATE
		${NETVM_JIT_DIR}/ia32/x86-asm.cpp
		${NETVM_JIT_DIR}/ia32/x86-emit.cpp
		${NETVM_JIT_DIR}/ia32/x86-backend.cpp
		${CMAKE_CURRENT_BINARY_DIR}/ia32/inssel-ia32.cpp
		${NETVM_JIT_DIR}/ia32/x86-regalloc.cpp
		${NETVM_JIT_DIR}/ia32/x86_switch_lowering.cpp
		${NETVM_JIT_DIR}/ia32/x86_counters.cpp
		${NETVM_JIT_DIR}/ia32/inssel-ia32.brg
		${NETVM_JIT_DIR}/ia32/x86-asm.h
		${NETVM_JIT_DIR}/ia32/x86-backend.h
		${NETVM_JIT_DIR}/ia32/x86-emit.h
		${CMAKE_CURRENT_BINARY_DIR}/ia32/inssel-ia32.h
		${NETVM_JIT_DIR}/ia32/x86-regalloc.h
		${NETVM_JIT_DIR}/ia32/x86_switch_lowering.h
		${NETVM_JIT_DIR}/ia32/x86_counters.h
		${X86DIS_SRC_DIR}/decode.c
		${X86DIS_SRC_DIR}/input.c
		${X86DIS_SRC_DIR}/itab.c
		${X86DIS_SRC_DIR}/syn.c
		${X86DIS_SRC_DIR}/syn-att.c
		${X86DIS_SRC_DIR}/syn-intel.c
		${X86DIS_SRC_DIR}/udis86.c
		${X86DIS_SRC_DIR}/decode.h
		${X86DIS_SRC_DIR}/extern.h
		${X86DIS_SRC_DIR}/input.h
		${X86DIS_SRC_DIR}/itab.h
		${X86DIS_SRC_DIR}/syn.h
		${X86DIS_SRC_DIR}/types.h
		${X86DIS_SRC_DIR}/udis86.h
	)

	target_include_directories(nbnetvm PRIVATE
		${NETVM_JIT_DIR}/ia32
		${X86DIS_SRC_DIR}
		${CMAKE_CURRENT_BINARY_DIR}/ia32
	)
endif()
#
# X11
#
if(ENABLE_X11_BACKEND)
	target_compile_definitions(nbnetvm PRIVATE ENABLE_X11_BACKEND)
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/x11/inssel-x11.cpp
		${CMAKE_CURRENT_BINARY_DIR}/x11/inssel-x11.h
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/x11
		COMMAND netvmburg ARGS -C X11InsSelector -p -d ${CMAKE_CURRENT_BINARY_DIR}/x11/inssel-x11.h ../../jit/x11/inssel-x11.brg > ${CMAKE_CURRENT_BINARY_DIR}/x11/inssel-x11.cpp
		DEPENDS netvmburg
		${NETVM_JIT_DIR}/x11/inssel-x11.brg
		WORKING_DIRECTORY "${NETVM_BASE_DIR}/tools/bin"
	)

	target_sources(nbnetvm PRIVATE
		${NETVM_JIT_DIR}/x11/x11-ir.cpp
		${NETVM_JIT_DIR}/x11/x11-backend.cpp
		${NETVM_JIT_DIR}/x11/x11-emit.cpp
		${NETVM_JIT_DIR}/x11/x11-util.cpp
		${NETVM_JIT_DIR}/x11/x11-optimization.cpp
		${NETVM_JIT_DIR}/x11/x11-coprocessor.cpp
		${CMAKE_CURRENT_BINARY_DIR}/x11/inssel-x11.cpp
		${NETVM_JIT_DIR}/x11/inssel-x11.brg
		${NETVM_JIT_DIR}/x11/x11-ir.h
		${CMAKE_CURRENT_BINARY_DIR}/x11/inssel-x11.h
		${NETVM_JIT_DIR}/x11/x11-ir.h
		${NETVM_JIT_DIR}/x11/x11-emit.h
		${NETVM_JIT_DIR}/x11/x11-optimization.h
		${NETVM_JIT_DIR}/x11/x11-coprocessor.h
		${NETVM_JIT_DIR}/x11/x11-util.h
		${NETVM_JIT_DIR}/x11/x11-backend.h
	)

	target_include_directories(nbnetvm PRIVATE ${NETVM_JIT_DIR}/x11 ${CMAKE_CURRENT_BINARY_DIR}/x11)
endif()

#
# octeon
#
if(ENABLE_OCTEON_BACKEND)
	target_compile_definitions(nbnetvm PRIVATE ENABLE_OCTEON_BACKEND)
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/octeon/inssel-octeon.cpp
		${CMAKE_CURRENT_BINARY_DIR}/octeon/inssel-octeon.h
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/octeon
		COMMAND netvmburg ARGS -C OcteonInsSelector -p -d ${CMAKE_CURRENT_BINARY_DIR}/octeon/inssel-octeon.h ../../jit/octeon/inssel-octeon.brg > ${CMAKE_CURRENT_BINARY_DIR}/octeon/inssel-octeon.cpp
		DEPENDS netvmburg
		${NETVM_JIT_DIR}/octeon/inssel-octeon.brg
		WORKING_DIRECTORY "${NETVM_BASE_DIR}/tools/bin"
	)
    
	target_sources(nbnetvm PRIVATE
		${NETVM_JIT_DIR}/octeon/octeon-asm.cpp
		${NETVM_JIT_DIR}/octeon/octeon-asm.inl
		${NETVM_JIT_DIR}/octeon/octeon-backend.cpp
		${NETVM_JIT_DIR}/octeon/octeon-regalloc.cpp
		${NETVM_JIT_DIR}/octeon/octeon-offset.cpp
		${NETVM_JIT_DIR}/octeon/octeon-emit.cpp
		${CMAKE_CURRENT_BINARY_DIR}/octeon/inssel-octeon.cpp
		${NETVM_JIT_DIR}/octeon/octeon-coprocessor.cpp
		${NETVM_JIT_DIR}/octeon/octeon-switch.cpp
		${NETVM_JIT_DIR}/octeon/inssel-octeon.brg
		${NETVM_JIT_DIR}/octeon/octeon-asm.h
		${NETVM_JIT_DIR}/octeon/octeon-backend.h
		${NETVM_JIT_DIR}/octeon/octeon-regalloc.h
		${NETVM_JIT_DIR}/octeon/octeon-offset.h
		${NETVM_JIT_DIR}/octeon/octeon-emit.h
		${CMAKE_CURRENT_BINARY_DIR}/octeon/inssel-octeon.h
		${NETVM_JIT_DIR}/octeon/octeon-coprocessor.h
		${NETVM_JIT_DIR}/octeon/octeon-switch.h
	)

	target_include_directories(nbnetvm PRIVATE ${NETVM_JIT_DIR}/octeon ${CMAKE_CURRENT_BINARY_DIR}/octeon)
endif()

#
# octeon-c
#
if(ENABLE_OCTEONC_BACKEND)
	target_compile_definitions(nbnetvm PRIVATE ENABLE_OCTEONC_BACKEND)
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/octeonc/inssel-octeonc.cpp
		${CMAKE_CURRENT_BINARY_DIR}/octeonc/inssel-octeonc.h
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/octeonc
		COMMAND netvmburg ARGS -C OcteoncInsSelector -p -d
		${CMAKE_CURRENT_BINARY_DIR}/octeonc/inssel-octeonc.h ../../jit/octeonc/inssel-octeonc.brg >
		${CMAKE_CURRENT_BINARY_DIR}/octeonc/inssel-octeonc.cpp
		DEPENDS netvmburg
		${NETVM_JIT_DIR}/octeonc/inssel-octeonc.brg
		WORKING_DIRECTORY "${NETVM_BASE_DIR}/tools/bin"
		)

	target_sources(nbnetvm PRIVATE
		${NETVM_JIT_DIR}/octeonc/octeonc-asm.cpp
		${NETVM_JIT_DIR}/octeonc/octeonc-asm.inl
		${NETVM_JIT_DIR}/octeonc/octeonc-backend.cpp
		${NETVM_JIT_DIR}/octeonc/octeonc-emit.cpp
		${CMAKE_CURRENT_BINARY_DIR}/octeonc/inssel-octeonc.cpp
		${NETVM_JIT_DIR}/octeonc/inssel-octeonc.brg
		### the next one is from octeon
		${NETVM_JIT_DIR}/octeon/octeon-coprocessor.cpp
		${NETVM_JIT_DIR}/octeonc/octeonc-asm.inl
		${NETVM_JIT_DIR}/octeonc/octeonc-asm.h
		${NETVM_JIT_DIR}/octeonc/octeonc-backend.h
		${NETVM_JIT_DIR}/octeonc/octeonc-emit.h
		${CMAKE_CURRENT_BINARY_DIR}/octeonc/inssel-octeonc.h
		### the next one is from octeon
		${NETVM_JIT_DIR}/octeon/octeon-coprocessor.h
	)

	target_include_directories(nbnetvm PRIVATE
		${NETVM_JIT_DIR}/octeonc
		${NETVM_JIT_DIR}/octeon
		${CMAKE_CURRENT_BINARY_DIR}/octeonc
	)
endif()

#
# X64
#
if(ENABLE_X64_BACKEND)
	target_compile_definitions(nbnetvm PRIVATE ENABLE_X64_BACKEND)
	set(X86DIS_SRC_DIR ${NETVM_JIT_DIR}/x86_x64_disasm )
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/x64/inssel-x64.cpp
		${CMAKE_CURRENT_BINARY_DIR}/x64/inssel-x64.h
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/x64
		COMMAND netvmburg
		ARGS
		#comment the following line to disable constant swapping
		-DSWAP_CONST
		-C X64InsSelector -p -d ${CMAKE_CURRENT_BINARY_DIR}/x64/inssel-x64.h ../../jit/x64/inssel-x64.brg > ${CMAKE_CURRENT_BINARY_DIR}/x64/inssel-x64.cpp
		DEPENDS netvmburg
		${NETVM_JIT_DIR}/x64/inssel-x64.brg
		WORKING_DIRECTORY "${NETVM_BASE_DIR}/tools/bin"
		)

	target_sources(nbnetvm PRIVATE
		${NETVM_JIT_DIR}/x64/x64-asm.cpp
		${NETVM_JIT_DIR}/x64/x64-emit.cpp
		${NETVM_JIT_DIR}/x64/x64-backend.cpp
		${NETVM_JIT_DIR}/x64/x64-opt.cpp
		${CMAKE_CURRENT_BINARY_DIR}/x64/inssel-x64.cpp
		${NETVM_JIT_DIR}/x64/x64-regalloc.cpp
		${NETVM_JIT_DIR}/x64/x64_switch_lowering.cpp
		${NETVM_JIT_DIR}/x64/x64_counters.cpp
		${NETVM_JIT_DIR}/x64/inssel-x64.brg
		${NETVM_JIT_DIR}/x64/x64-asm.h
		${NETVM_JIT_DIR}/x64/x64-backend.h
		${NETVM_JIT_DIR}/x64/x64-emit.h
		${NETVM_JIT_DIR}/x64/x64-opt.h
		${CMAKE_CURRENT_BINARY_DIR}/x64/inssel-x64.h
		${NETVM_JIT_DIR}/x64/x64-regalloc.h
		${NETVM_JIT_DIR}/x64/x64_switch_lowering.h
		${NETVM_JIT_DIR}/x64/x64_counters.h
		${X86DIS_SRC_DIR}/decode.c
		${X86DIS_SRC_DIR}/input.c
		${X86DIS_SRC_DIR}/itab.c
		${X86DIS_SRC_DIR}/syn.c
		${X86DIS_SRC_DIR}/syn-att.c
		${X86DIS_SRC_DIR}/syn-intel.c
		${X86DIS_SRC_DIR}/udis86.c
		${X86DIS_SRC_DIR}/decode.h
		${X86DIS_SRC_DIR}/extern.h
		${X86DIS_SRC_DIR}/input.h
		${X86DIS_SRC_DIR}/itab.h
		${X86DIS_SRC_DIR}/syn.h
		${X86DIS_SRC_DIR}/types.h
		${X86DIS_SRC_DIR}/udis86.h
	)

	target_include_directories(nbnetvm PRIVATE
		${NETVM_JIT_DIR}/x64
		${X86DIS_SRC_DIR}
		${CMAKE_CURRENT_BINARY_DIR}/x64
	)
endif()

# Platform-specific definitions
if(WIN32)
	target_compile_definitions(nbnetvm PRIVATE
		_CRT_SECURE_NO_WARNINGS
		_CRT_SECURE_NO_DEPRECATE
		_SCL_SECURE_NO_DEPRECATE
		WIN32_LEAN_AND_MEAN
		NETVM_EXPORTS
		__STDC__
	)
endif()

target_compile_definitions(nbnetvm PRIVATE AVAILABLE_COPROS_NO=${AVAILABLE_COPROS_NO})

######################################################################################################################
#
# libraries linked with netvm
#

target_link_libraries(nbnetvm PRIVATE netbeeOptions pcre $<$<BOOL:${PCAP_FOUND}>:pcap>)
if(WIN32)
	target_link_libraries(nbnetvm PRIVATE ws2_32.lib)
endif()
