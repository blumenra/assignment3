cmake_minimum_required(VERSION 3.6)
project(Client)
#CFLAGS(-g -Wall -Weffc++ -std=c++11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Weffc++")
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    include/BidiEncDec.h
    include/BidiMessage.h
    include/connectionHandler.h
    include/bidiInputConverter.h
    src/BidiEncDec.cpp
    src/BidiMessage.cpp
    src/client.cpp
    src/connectionHandler.cpp
    src/bidiInputConverter.cpp)

add_executable(Client ${SOURCE_FILES})