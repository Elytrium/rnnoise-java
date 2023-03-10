cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(CMAKE_C_COMPILER "clang")

project(rnnoise-native LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(LIB_OUTPUT_DIR ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR})

find_package(Java REQUIRED)
set(JAVA_AWT_LIBRARY NotNeeded)
set(JAVA_JVM_LIBRARY NotNeeded)
find_package(JNI REQUIRED)

include_directories(${JNI_INCLUDE_DIRS})

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "
    CMake generation is not allowed within the source directory!
    Remove the CMakeCache.txt file and try again from another folder, e.g.:
    mkdir build && cd build
    cmake ..
  ")
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS_RELEASE "-O2 -Wall -Werror -Wno-null-dereference -Wno-deprecated-declarations")
set(CMAKE_SHARED_LIBRARY_PREFIX "lib")

if (WIN32)
  add_definitions(-D_USE_MATH_DEFINES)
endif()

include_directories(include rnnoise/include)
file(GLOB SOURCES src/*.c rnnoise/src/*.c)
add_library(rnnoise-native SHARED ${SOURCES})

if (WIN32)
  target_link_libraries(rnnoise-native "-lmsvcrt")
endif()
