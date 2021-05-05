#--- check build type
cmake_minimum_required(VERSION 3.10)
project(opengcl VERSION 1.0.0)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#--- check buil type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug, Release, or MinSizeRel." FORCE)
    message(STATUS "No build type specified, defaulting to MinSizeRel.")
endif()

#--- check target architecture
if(NOT TARGET_ARCH)
    set(TARGET_ARCH "x86_64" CACHE STRING "Choose target architecture, options are: x86, x86_64..." FORCE)
    message(STATUS "No build type specified, defaulting to MinSizeRel.")
endif()

if(${TARGET_ARCH} STREQUAL "x64")
    set(TARGET_ARCH "x86_64" CACHE STRING "Choose target architecture, options are: x86, x86_64..." FORCE)
endif()

#--- check system information
message(STATUS "Generator ......... ${CMAKE_GENERATOR}")
message(STATUS "Build Type ........ ${CMAKE_BUILD_TYPE}")
message(STATUS "Target Arch ....... ${TARGET_ARCH}")
if(MSVC)
    message(STATUS "MSVC Version ...... ${MSVC_VERSION}")
    include(install_pthread)      # download pthread4w library
endif()


#------------------------------------------------------------------------------
# opengcl library
#------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)


#--- Compiler settings
add_compile_definitions($<$<BOOL:WIN32>:GCL_EXPORTS>)
add_compile_definitions($<$<BOOL:MINGW>:__USE_MINGW_ANSI_STDIO>)   # fix printf format string warnings
add_compile_definitions($<$<BOOL:MSVC>:_CRT_SECURE_NO_WARNINGS>)
add_compile_definitions($<$<BOOL:MSVC>:_WINSOCK_DEPRECATED_NO_WARNINGS>)
if (BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG _DEBUG)
endif()

if(MSVC)
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /errorReport:none /nologo ")
    add_compile_options(
        /W4 /WX
        /errorReport:none /nologo
    )
else()
    add_compile_options(
        -fPIC
        -Wall -pedantic -Werror
    )
endif()


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
add_library(${PROJECT_NAME} SHARED ${SRC})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
if (WIN32)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
else()
    target_link_libraries(${PROJECT_NAME} pthread)
endif()

add_subdirectory(samples)
