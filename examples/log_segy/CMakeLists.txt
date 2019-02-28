cmake_minimum_required (VERSION 3.0)

project(SegyLogger C CXX)

find_package(Threads REQUIRED)

add_executable(SegyLogger segy_logger.cpp)

target_link_libraries(SegyLogger liberad liberadfile ${CMAKE_THREAD_LIBS_INIT})
