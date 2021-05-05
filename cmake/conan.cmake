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
