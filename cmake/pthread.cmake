# install external libraries using conan: pthreads4w

#message(STATUS)
    # set(SAVE_CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    # set(CMAKE_BUILD_TYPE ${BUILD_TYPE})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/${CMAK_BUILD_TYPE}/include/pthread.h")
    include(conan)

    conan_cmake_configure(REQUIRES pthreads4w/3.0.0
        GENERATORS cmake
        OPTIONS pthreads4w:shared=True
        IMPORTS "include, * -> ./include/pthread"
        IMPORTS "bin, *.dll -> ./bin"
        IMPORTS "lib, *.lib* -> ./lib"
    )

    conan_cmake_autodetect(settings)
    # message(STATUS settings=====>${settings})
    conan_cmake_install(PATH_OR_REFERENCE . SETTINGS ${settings})   # install using conanfile.txt
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()
