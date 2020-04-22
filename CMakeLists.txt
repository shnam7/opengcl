cmake_minimum_required(VERSION 3.10.2)

project(opengcl VERSION 1.0.0)

#--- Compiler options
# set (CMAKE_CXX_STANDARD 11)
set(COMPILE_OPTIONS -O3 -Wall -fomit-frame-pointer -MMD)
if (WIN32)
    set(COMPILE_OPTIONS -DGCL_EXPORTS)
endif()
if (MSVC)
    set(COMPILE_OPTIONS ${COMPILE_OPTIONS} -D_CRT_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()
add_compile_options(${COMPILE_OPTIONS})
add_compile_definitions(${COMPILE_DEFINITIONS})


#--- source files
include_directories(include)

set(SRC
    src/gtime.c
	src/gque.c
	src/gmbox.c
	src/gmmap.cpp
	src/gsocket.c
	src/gsocket_x.cpp
	src/gthread_x.cpp
	src/gtok.c
	src/gutil.c
	src/gtime.c
	src/gthread.c
)
set(SAMPLES
	samples/ex-time.cpp
    samples/ex-list.cpp
	samples/ex-que.cpp
	samples/ex-mmap.cpp
	samples/ex-mbox.cpp
	samples/ex-socket.cpp
	samples/ex-thread.c
	samples/ex-thread_x.cpp
	samples/ex-tok.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND SRC src/gdmsg.c)
endif()

# Microsoft Visual Studio support
if (MSVC)
    include_directories(src/win32)
	list(APPEND SRC
		src/win32/gsem.c
		src/win32/gthread.c
		src/win32/gthread_cond.c
		src/win32/gthread_mutex.c
		src/win32/gthread_rwlock.c
		src/win32/gthread_self.c
		src/win32/opengcl_dll.cpp
    )
    list(REMOVE_ITEM SRC src/gthread.c)
endif()

# gthread not supported in MINGW (gtime, gmbox depends on gthread)
if (MINGW)
    list(REMOVE_ITEM SRC
        src/gtime.c
        # src/gque.c
        src/gmbox.c
        src/gmmap.cpp
        src/gsocket.c
        src/gsocket_x.cpp
        src/gthread_x.cpp
        src/gtok.c
        src/gutil.c
        src/gtime.c
        src/gthread.c
    )
    list(REMOVE_ITEM SAMPLES
        samples/ex-time.cpp
        samples/ex-list.cpp
        # samples/ex-que.cpp
        samples/ex-mmap.cpp
        samples/ex-mbox.cpp
        samples/ex-socket.cpp
        samples/ex-thread.c
        samples/ex-thread_x.cpp
        samples/ex-tok.cpp
    )
endif()


#--- Build opengcl library
add_library(opengcl SHARED ${SRC})
if(WIN32)
    target_link_libraries(opengcl wsock32 ws2_32)
else()
    target_link_libraries(pthread)
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
