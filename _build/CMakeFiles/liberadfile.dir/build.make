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
CMAKE_SOURCE_DIR = /home/viva/software/liberad/liberadfile_test/liberadfile

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/viva/software/liberad/liberadfile_test/liberadfile/_build

# Include any dependencies generated for this target.
include CMakeFiles/liberadfile.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/liberadfile.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/liberadfile.dir/flags.make

CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o: CMakeFiles/liberadfile.dir/flags.make
CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o: ../src/liberadfile.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/viva/software/liberad/liberadfile_test/liberadfile/_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o -c /home/viva/software/liberad/liberadfile_test/liberadfile/src/liberadfile.cpp

CMakeFiles/liberadfile.dir/src/liberadfile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/liberadfile.dir/src/liberadfile.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/viva/software/liberad/liberadfile_test/liberadfile/src/liberadfile.cpp > CMakeFiles/liberadfile.dir/src/liberadfile.cpp.i

CMakeFiles/liberadfile.dir/src/liberadfile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/liberadfile.dir/src/liberadfile.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/viva/software/liberad/liberadfile_test/liberadfile/src/liberadfile.cpp -o CMakeFiles/liberadfile.dir/src/liberadfile.cpp.s

CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o.requires:

.PHONY : CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o.requires

CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o.provides: CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o.requires
	$(MAKE) -f CMakeFiles/liberadfile.dir/build.make CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o.provides.build
.PHONY : CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o.provides

CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o.provides.build: CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o


# Object files for target liberadfile
liberadfile_OBJECTS = \
"CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o"

# External object files for target liberadfile
liberadfile_EXTERNAL_OBJECTS =

libliberadfile.so.1.0.0: CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o
libliberadfile.so.1.0.0: CMakeFiles/liberadfile.dir/build.make
libliberadfile.so.1.0.0: CMakeFiles/liberadfile.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/viva/software/liberad/liberadfile_test/liberadfile/_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libliberadfile.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/liberadfile.dir/link.txt --verbose=$(VERBOSE)
	$(CMAKE_COMMAND) -E cmake_symlink_library libliberadfile.so.1.0.0 libliberadfile.so.1 libliberadfile.so

libliberadfile.so.1: libliberadfile.so.1.0.0
	@$(CMAKE_COMMAND) -E touch_nocreate libliberadfile.so.1

libliberadfile.so: libliberadfile.so.1.0.0
	@$(CMAKE_COMMAND) -E touch_nocreate libliberadfile.so

# Rule to build all files generated by this target.
CMakeFiles/liberadfile.dir/build: libliberadfile.so

.PHONY : CMakeFiles/liberadfile.dir/build

CMakeFiles/liberadfile.dir/requires: CMakeFiles/liberadfile.dir/src/liberadfile.cpp.o.requires

.PHONY : CMakeFiles/liberadfile.dir/requires

CMakeFiles/liberadfile.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/liberadfile.dir/cmake_clean.cmake
.PHONY : CMakeFiles/liberadfile.dir/clean

CMakeFiles/liberadfile.dir/depend:
	cd /home/viva/software/liberad/liberadfile_test/liberadfile/_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/viva/software/liberad/liberadfile_test/liberadfile /home/viva/software/liberad/liberadfile_test/liberadfile /home/viva/software/liberad/liberadfile_test/liberadfile/_build /home/viva/software/liberad/liberadfile_test/liberadfile/_build /home/viva/software/liberad/liberadfile_test/liberadfile/_build/CMakeFiles/liberadfile.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/liberadfile.dir/depend
