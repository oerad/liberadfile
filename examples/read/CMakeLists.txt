cmake_minimum_required (VERSION 3.0)

project(Reader C CXX)

add_executable(Reader reader.cpp)

target_link_libraries(Reader liberadfile)
