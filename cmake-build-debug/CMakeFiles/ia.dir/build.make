# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /cygdrive/c/Users/eddox/.CLion2017.1/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/eddox/.CLion2017.1/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Users/eddox/Documents/GitHub/IA_Proyect

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Users/eddox/Documents/GitHub/IA_Proyect/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ia.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ia.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ia.dir/flags.make

CMakeFiles/ia.dir/main.cpp.o: CMakeFiles/ia.dir/flags.make
CMakeFiles/ia.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/eddox/Documents/GitHub/IA_Proyect/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ia.dir/main.cpp.o"
	/usr/bin/c++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ia.dir/main.cpp.o -c /cygdrive/c/Users/eddox/Documents/GitHub/IA_Proyect/main.cpp

CMakeFiles/ia.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ia.dir/main.cpp.i"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/c/Users/eddox/Documents/GitHub/IA_Proyect/main.cpp > CMakeFiles/ia.dir/main.cpp.i

CMakeFiles/ia.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ia.dir/main.cpp.s"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/c/Users/eddox/Documents/GitHub/IA_Proyect/main.cpp -o CMakeFiles/ia.dir/main.cpp.s

CMakeFiles/ia.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/ia.dir/main.cpp.o.requires

CMakeFiles/ia.dir/main.cpp.o.provides: CMakeFiles/ia.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/ia.dir/build.make CMakeFiles/ia.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/ia.dir/main.cpp.o.provides

CMakeFiles/ia.dir/main.cpp.o.provides.build: CMakeFiles/ia.dir/main.cpp.o


# Object files for target ia
ia_OBJECTS = \
"CMakeFiles/ia.dir/main.cpp.o"

# External object files for target ia
ia_EXTERNAL_OBJECTS =

ia.exe: CMakeFiles/ia.dir/main.cpp.o
ia.exe: CMakeFiles/ia.dir/build.make
ia.exe: CMakeFiles/ia.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Users/eddox/Documents/GitHub/IA_Proyect/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ia.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ia.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ia.dir/build: ia.exe

.PHONY : CMakeFiles/ia.dir/build

CMakeFiles/ia.dir/requires: CMakeFiles/ia.dir/main.cpp.o.requires

.PHONY : CMakeFiles/ia.dir/requires

CMakeFiles/ia.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ia.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ia.dir/clean

CMakeFiles/ia.dir/depend:
	cd /cygdrive/c/Users/eddox/Documents/GitHub/IA_Proyect/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Users/eddox/Documents/GitHub/IA_Proyect /cygdrive/c/Users/eddox/Documents/GitHub/IA_Proyect /cygdrive/c/Users/eddox/Documents/GitHub/IA_Proyect/cmake-build-debug /cygdrive/c/Users/eddox/Documents/GitHub/IA_Proyect/cmake-build-debug /cygdrive/c/Users/eddox/Documents/GitHub/IA_Proyect/cmake-build-debug/CMakeFiles/ia.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ia.dir/depend

