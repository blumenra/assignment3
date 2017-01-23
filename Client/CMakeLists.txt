cmake_minimum_required(VERSION 3.6)
project(Client)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/BidiEncDec.h
        include/bidiInputConverter.h
        include/BidiMessage.h
        include/ClientProtocol.h
        include/connectionHandler.h
        src/BidiEncDec.cpp
        src/bidiInputConverter.cpp
        src/BidiMessage.cpp
        src/client.cpp
        src/ClientProtocol.cpp
        src/connectionHandler.cpp)

add_executable(Client ${SOURCE_FILES})