# install pthread library for windows

#--- library already loaded
if(CONAN_INCLUDE_DIRS MATCHES ".*pthread.*")
    message(STATUS "pthreads4w ....... available.")
    return()
endif()

#--- library installed but not loaded
if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    if(CONAN_INCLUDE_DIRS MATCHES ".*pthread.*")
        include_directories(${CONAN_INCLUDE_DIRS})
        link_directories(${CONAN_LIB_DIRS})
        message(STATUS "pthreads4w ....... ready")
        return()
    endif()
endif()

#--- library not installed
message(STATUS "Installing pthreads4w ....... ")
include(conan)

conan_cmake_configure(
    REQUIRES pthreads4w/3.0.0
    GENERATORS cmake
    OPTIONS pthreads4w:shared=True
    # IMPORTS "include, * -> $./include/pthread"
    # IMPORTS "lib, *.lib* -> ./lib"
    IMPORTS "bin, *.dll -> ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
)

conan_cmake_install(PATH_OR_REFERENCE .
    SETTINGS arch=${TARGET_ARCH} build_type=${CMAKE_BUILD_TYPE}
)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include_directories(${CONAN_INCLUDE_DIRS})
link_directories(${CONAN_LIB_DIRS})
# conan_basic_setup()
