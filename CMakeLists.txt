cmake_minimum_required (VERSION 3.0)

#info about liberadfile
set(LIBERADFILE_MAJOR 1)
set(LIBERADFILE_MINOR 0)
set(LIBERADFILE_PATCH 0)
set(LIBERADFILE_VERSION ${LIBERADFILE_MAJOR}.${LIBERADFILE_MINOR}.${LIBERADFILE_PATCH})
set(LIBERADFILE_DESCRIPTION "Liberadfile is a free open-source library for logging and reading GPR data produced by Oerad hardware.")
set(LIBERADFILE_PACKAGE_NAME "liberadfile-${LIBERADFILE_VERSION}")

project (liberadfile
        LANGUAGES CXX
        VERSION ${LIBERADFILE_VERSION}
        DESCRIPTION ${LIBERADFILE_DESCRIPTION})


include(GNUInstallDirs)

add_library(liberadfile SHARED
            src/liberadfile.cpp)

#target_link_libraries(liberadfile usb-1.0)

set_target_properties(liberadfile PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/liberadfile.h
    PRIVATE_HEADER include/erad.h
    PRIVATE_HEADER include/segy.h)

configure_file(liberadfile.pc.in liberadfile.pc @ONLY)

target_include_directories(liberadfile PRIVATE .)

install(TARGETS liberadfile
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/liberadfile
        PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/liberadfile)

install (FILES ${CMAKE_BINARY_DIR}/liberadfile.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
