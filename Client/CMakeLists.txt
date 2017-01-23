cmake_minimum_required(VERSION 3.6)
project(Client)

#CFLAGS(-g -Wall -Weffc++ -std=c++11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Weffc++")
set(CMAKE_CXX_STANDARD 11)

find_package(Boost 1.53.0 COMPONENTS
        system filesystem thread REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

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

add_executable(Boost_Client src/client.cpp ${SOURCE_FILES})

target_link_libraries(Boost_Client ${Boost_LIBRARIES})