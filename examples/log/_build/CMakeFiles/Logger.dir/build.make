# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/viva/software/liberad/liberadfile_test/liberadfile/examples/log

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/viva/software/liberad/liberadfile_test/liberadfile/examples/log/_build

# Include any dependencies generated for this target.
include CMakeFiles/Logger.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Logger.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Logger.dir/flags.make

CMakeFiles/Logger.dir/logger.cpp.o: CMakeFiles/Logger.dir/flags.make
CMakeFiles/Logger.dir/logger.cpp.o: ../logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/viva/software/liberad/liberadfile_test/liberadfile/examples/log/_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Logger.dir/logger.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Logger.dir/logger.cpp.o -c /home/viva/software/liberad/liberadfile_test/liberadfile/examples/log/logger.cpp

CMakeFiles/Logger.dir/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Logger.dir/logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/viva/software/liberad/liberadfile_test/liberadfile/examples/log/logger.cpp > CMakeFiles/Logger.dir/logger.cpp.i

CMakeFiles/Logger.dir/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Logger.dir/logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/viva/software/liberad/liberadfile_test/liberadfile/examples/log/logger.cpp -o CMakeFiles/Logger.dir/logger.cpp.s

CMakeFiles/Logger.dir/logger.cpp.o.requires:

.PHONY : CMakeFiles/Logger.dir/logger.cpp.o.requires

CMakeFiles/Logger.dir/logger.cpp.o.provides: CMakeFiles/Logger.dir/logger.cpp.o.requires
	$(MAKE) -f CMakeFiles/Logger.dir/build.make CMakeFiles/Logger.dir/logger.cpp.o.provides.build
.PHONY : CMakeFiles/Logger.dir/logger.cpp.o.provides

CMakeFiles/Logger.dir/logger.cpp.o.provides.build: CMakeFiles/Logger.dir/logger.cpp.o


# Object files for target Logger
Logger_OBJECTS = \
"CMakeFiles/Logger.dir/logger.cpp.o"

# External object files for target Logger
Logger_EXTERNAL_OBJECTS =

Logger: CMakeFiles/Logger.dir/logger.cpp.o
Logger: CMakeFiles/Logger.dir/build.make
Logger: CMakeFiles/Logger.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/viva/software/liberad/liberadfile_test/liberadfile/examples/log/_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Logger"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Logger.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Logger.dir/build: Logger

.PHONY : CMakeFiles/Logger.dir/build

CMakeFiles/Logger.dir/requires: CMakeFiles/Logger.dir/logger.cpp.o.requires

.PHONY : CMakeFiles/Logger.dir/requires

CMakeFiles/Logger.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Logger.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Logger.dir/clean

CMakeFiles/Logger.dir/depend:
	cd /home/viva/software/liberad/liberadfile_test/liberadfile/examples/log/_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/viva/software/liberad/liberadfile_test/liberadfile/examples/log /home/viva/software/liberad/liberadfile_test/liberadfile/examples/log /home/viva/software/liberad/liberadfile_test/liberadfile/examples/log/_build /home/viva/software/liberad/liberadfile_test/liberadfile/examples/log/_build /home/viva/software/liberad/liberadfile_test/liberadfile/examples/log/_build/CMakeFiles/Logger.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Logger.dir/depend

