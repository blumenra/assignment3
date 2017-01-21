cmake_minimum_required(VERSION 3.6)
project(Client)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    cmake-build-debug/CMakeFiles/3.6.3/CompilerIdC/CMakeCCompilerId.c
    cmake-build-debug/CMakeFiles/3.6.3/CompilerIdCXX/CMakeCXXCompilerId.cpp
    cmake-build-debug/CMakeFiles/feature_tests.c
    cmake-build-debug/CMakeFiles/feature_tests.cxx
    include/BidiEncDec.h
    include/BidiMessage.h
    include/connectionHandler.h
    src/BidiEncDec.cpp
    src/BidiMessage.cpp
    src/client.cpp
    src/connectionHandler.cpp)

add_executable(Client ${SOURCE_FILES})