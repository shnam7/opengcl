#------------------------------------------------------------------------------
# samples programs
#------------------------------------------------------------------------------
set(SAMPLES
	ex-time.cpp
    ex-list.cpp
	ex-que.cpp
	ex-mmap.cpp
	ex-tok.cpp
	ex-sem.cpp
	ex-thread.cpp
	ex-pthread.cpp
	ex-mbox.cpp
	ex-event.cpp
	ex-socket.cpp
)

#--- Build samples
set(sampleTargetNames)	# list of sample program names
set(LIBS opengcl ${CONAN_LIBS})

macro(add_example pathName)
	get_filename_component(fileName  ${pathName} NAME)	# file name without path
	get_filename_component(name  ${fileName} NAME_WE)	# file name without extension

	# message("Build ${name} from ${fileName}....")
	add_executable(${name} ${fileName})
	target_compile_definitions(${name} PUBLIC _REENTRANT)
    target_link_libraries(${name} ${LIBS})
	# set_target_properties(${name} PROPERTIES EXCLUDE_FROM_ALL 1)

	list(APPEND sampleTargetNames ${name})
endmacro(add_example)


foreach(exFile ${SAMPLES})
	add_example(${exName} ${exFile})
endforeach(exFile ${SAMPLES})

add_custom_target(samples DEPENDS ${sampleTargetNames})
