cmake_minimum_required(VERSION 3.10.2)
project(OpenGCL VERSION 1.0.0)


set(EXCLUDE_DIR __trash)

#--- Compiler options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add_compile_options($<$<CONFIG:DEBUG>:-Wall>)     # -fomit-frame-pointer
add_compile_definitions($<$<CONFIG:DEBUG>:_DEBUG>)
add_compile_definitions($<$<BOOL:WIN32>:GCL_EXPORTS>)
add_compile_definitions($<$<BOOL:MINGW>:__USE_MINGW_ANSI_STDIO>)   # fix printf format string warnings
add_compile_definitions($<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>)
add_compile_definitions($<$<CXX_COMPILER_ID:MSVC>:_WINSOCK_DEPRECATED_NO_WARNINGS>)

include_directories(include src/gthread/include)


#--- source files
set (SRC_COMMON
    src/gtimer.cpp
    src/gque.cpp
    src/glist.cpp
	src/gmmap.cpp
	src/gsocket.cpp
	src/gtockenizer.cpp
	src/gutil.cpp
	src/gthread.cpp
    src/gsem.cpp
    src/gevent.cpp
)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND SRC_COMMON src/gdmsg.cpp)
endif()

set(SRC_GTHREAD
    src/gthread/src/gthread_mutex.cpp
    src/gthread/src/gthread_cond.cpp
    src/gthread/src/gthread_rwlock.cpp
    src/gthread/src/_gthread.cpp
    src/gthread/src/_gthread_self.cpp
    src/gthread/src/_gthread_sleep.cpp
)

# samples source
set(SAMPLES
	samples/ex-timer.cpp
    samples/ex-list.cpp
	samples/ex-que.cpp
	samples/ex-mmap.cpp
	samples/ex-tok.cpp
	samples/ex-sem.cpp
	samples/ex-thread.cpp
	samples/ex-mbox.cpp
	samples/ex-event.cpp
	samples/ex-socket.cpp
)

set (SRC ${SRC_COMMON} ${SRC_GTHREAD})
if (WIN32)  # NOT for CYGWIN
    list(APPEND SRC src/win32/opengcl_dll.cpp )
endif()


#--- Build opengcl library
add_library(opengcl SHARED ${SRC})
if(WIN32)
    target_link_libraries(opengcl wsock32 ws2_32)
else()
    # target_link_libraries(pthread)
endif()


#--- Build samples
set(sampleTargetNames)	# list of sample program names

macro(add_example pathName)
	get_filename_component(fileName  ${pathName} NAME)	# file name without path
	get_filename_component(name  ${fileName} NAME_WE)	# file name without extension

	# message("Build ${name} from ${fileName}....")
	add_executable(${name} samples/${fileName})
	target_compile_definitions(${name} PUBLIC _REENTRANT)
    target_link_libraries(${name} opengcl)
    if(NOT WIN32 AND NOT MINGW)
        target_link_libraries(${name} pthread)
    endif()

	# set_target_properties(${name} PROPERTIES EXCLUDE_FROM_ALL 1)
	list(APPEND sampleTargetNames ${name})
endmacro(add_example)

foreach(exFile ${SAMPLES})
	add_example(${exName} ${exFile})
endforeach(exFile ${SAMPLES})

add_custom_target(samples DEPENDS ${sampleTargetNames})
