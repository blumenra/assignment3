cmake_minimum_required(VERSION 3.6)
project(Client)

set(CMAKE_CXX_STANDARD 11)



#find_package(Boost 1.58.0 COMPONENTS system filesystem REQUIRED)
#
#if(Boost_FOUND)
#
#    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
#    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
#    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
#
#    include_directories(${Boost_INCLUDE_DIRS})
#
#endif()
#
##add_executable(Client ${SOURCE_FILES})
#
#if(Boost_FOUND)
#
##    target_link_libraries(Client ${Boost_LIBRARIES})
#
#endif()

set(SOURCE_FILES
        include/connectionHandler.h
        src/client.cpp
        src/connectionHandler.cpp include/BidiEncDec.h include/BidiMessage.h src/BidiMessage.cpp)
