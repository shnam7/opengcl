cmake_minimum_required(VERSION 3.10.2)
project(OpenGCL VERSION 1.0.0)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(EXCLUDE_DIR __trash)

#--- Compiler options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options($<$<CONFIG:DEBUG>:-Wall>)     # -fomit-frame-pointer
add_compile_definitions($<$<CONFIG:DEBUG>:_DEBUG>)
add_compile_definitions($<$<BOOL:WIN32>:GCL_EXPORTS>)
add_compile_definitions($<$<BOOL:MINGW>:__USE_MINGW_ANSI_STDIO>)   # fix printf format string warnings
add_compile_definitions($<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>)
add_compile_definitions($<$<CXX_COMPILER_ID:MSVC>:_WINSOCK_DEPRECATED_NO_WARNINGS>)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

if(MSVC)
#     message(STATUS "MSVC Version ...... ${MSVC_VERSION}")
#     message(STATUS "Installing external libraries using conan ......>>>>>")
#     # add_compile_options(
#     #     $<$<CONFIG:>:/MD> #---------|
#     #     $<$<CONFIG:Debug>:/MDd> #---|-- Statically link the runtime libraries
#     #     $<$<CONFIG:Release>:/MD> #--|
#     # )
    include(pthread)      # install external libs (pthreads4w,)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()


include_directories(include build/include)


#------------------------------------------------------------------------------
# opengcl library
#------------------------------------------------------------------------------
set (SRC
    src/gque.cpp
    src/glist.cpp
	src/gmmap.cpp
	src/gsocket.cpp
	src/gtockenizer.cpp
	src/gthread.cpp
    src/gevent.cpp
	src/gtime.cpp
    src/gtime_sleep.cpp
	src/util/gutil.cpp
	src/util/filelength.cpp
    $<$<CONFIG:DEBUG>:src/gdmsg.cpp>
    $<$<CONFIG:WIN32>:src/win32/opengcl_dll.cpp>
)

#--- Build opengcl library
add_library(opengcl SHARED ${SRC})
target_link_libraries(opengcl ${CONAN_LIBS})
if (WIN32)
    target_link_libraries(opengcl wsock32 ws2_32)
else()
    target_link_libraries(opengcl pthread)
endif()


#------------------------------------------------------------------------------
# samples programs
#------------------------------------------------------------------------------
set(SAMPLES
	samples/ex-time.cpp
    samples/ex-list.cpp
	samples/ex-que.cpp
	samples/ex-mmap.cpp
	samples/ex-tok.cpp
	samples/ex-sem.cpp
	samples/ex-thread.cpp
	samples/ex-pthread.cpp
	samples/ex-mbox.cpp
	samples/ex-event.cpp
	samples/ex-socket.cpp
)

#--- Build samples
set(sampleTargetNames)	# list of sample program names
set(LIBS opengcl ${CONAN_LIBS})

macro(add_example pathName)
	get_filename_component(fileName  ${pathName} NAME)	# file name without path
	get_filename_component(name  ${fileName} NAME_WE)	# file name without extension

	# message("Build ${name} from ${fileName}....")
	add_executable(${name} samples/${fileName})
	target_compile_definitions(${name} PUBLIC _REENTRANT)
    target_link_libraries(${name} ${LIBS})
	# set_target_properties(${name} PROPERTIES EXCLUDE_FROM_ALL 1)

	list(APPEND sampleTargetNames ${name})
endmacro(add_example)


foreach(exFile ${SAMPLES})
	add_example(${exName} ${exFile})
endforeach(exFile ${SAMPLES})

add_custom_target(samples DEPENDS ${sampleTargetNames})
