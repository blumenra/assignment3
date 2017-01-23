cmake_minimum_required(VERSION 3.6)
project(Client)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost 1.53.0 COMPONENTS
        system filesystem thread REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

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

add_executable(Boost_Client src/client.cpp ${SOURCE_FILES})
target_link_libraries(Boost_Client ${Boost_LIBRARIES})