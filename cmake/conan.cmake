# install external libraries using conan: pthreads4w

# Download conan.cmake file
# ref: https://github.com/conan-io/cmake-conan
# ref: https://docs.conan.io/en/latest/howtos/cmake_launch.html
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON)
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)

# install pthreads4w lib
conan_cmake_configure(REQUIRES pthreads4w/3.0.0 GENERATORS cmake
                    OPTIONS pthreads4w:shared=True
                    IMPORTS "bin, *.dll -> ./bin"
                    IMPORTS "lib, *.dylib* -> ./bin")

set(SAVE_CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE Release)
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE . SETTINGS ${settings})   # install using conanfile.txt
set(CMAKE_BUILD_TYPE ${SAVE_CMAKE_BUILD_TYPE})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# message("CONAN_LIBS: " ${CONAN_LIBS})
# target_link_libraries(ex-pthread pthread4w::pthread4w)
# target_link_libraries(ex-pthread ${CONAN_LIBS})
