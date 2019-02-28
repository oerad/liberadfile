cmake_minimum_required (VERSION 3.0)

project(Logger C CXX)

find_package(Threads REQUIRED)

add_executable(Logger logger.cpp)

target_link_libraries(Logger liberad liberadfile ${CMAKE_THREAD_LIBS_INIT})
