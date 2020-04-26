cmake_minimum_required(VERSION 3.10.2)

project(opengcl VERSION 1.0.0)
set (EXCLUDE_DIR __trash)

#--- Compiler options
# set (CMAKE_CXX_STANDARD 11)
set(COMPILE_OPTIONS -O3 -Wall -fomit-frame-pointer -MMD)
if (WIN32)
    set(COMPILE_OPTIONS -DGCL_EXPORTS)
endif()
if (MSVC)
    set(COMPILE_OPTIONS ${COMPILE_OPTIONS} -D_CRT_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

#--- variables
add_compile_options(${COMPILE_OPTIONS})
add_compile_definitions(${COMPILE_DEFINITIONS})
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
	samples/ex-socket.cpp
)

set (SRC ${SRC_COMMON} ${SRC_GTHREAD})


# Microsoft Visual Studio support
if (WIN32)  # NOT for CYGWIN
    list(APPEND SRC src/win32/opengcl_dll.cpp )
endif()

# gthread not supported in MINGW (gtime, gmbox depends on gthread)
# if (MINGW)
#     list(REMOVE_ITEM SRC
#         # src/gmbox.cpp
#         # src/gsocket.cpp
#         # src/gsocket_x.cpp
#         # src/gthread_x.cpp
#         # src/gtok.cpp
#         # src/gutil.cpp
#         # src/gthread.cpp
#     )
#     list(REMOVE_ITEM SAMPLES
#         # samples/ex-mbox.cpp
#         # samples/ex-socket.cpp
#         # samples/ex-thread.c
#         # samples/ex-thread_x.cpp
#         # samples/ex-tok.cpp
#     )
# endif()


#--- Build opengcl library
add_library(opengcl SHARED ${SRC})
if(WIN32)
    target_link_libraries(opengcl wsock32 ws2_32)
else()
    # target_link_libraries(pthread)
endif()

# target_compile_definitions(opengcl PUBLIC ${COMPILE_DEFINITIONS})


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
