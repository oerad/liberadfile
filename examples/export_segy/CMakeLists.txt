cmake_minimum_required (VERSION 3.0)

project(Exporter C CXX)

add_executable(Exporter export_segy.cpp)

target_link_libraries(Exporter liberadfile)
