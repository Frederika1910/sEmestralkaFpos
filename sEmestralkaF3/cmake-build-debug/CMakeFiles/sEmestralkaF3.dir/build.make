# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/tmp.uYxTz0VSd9

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.uYxTz0VSd9/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/sEmestralkaF3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sEmestralkaF3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sEmestralkaF3.dir/flags.make

CMakeFiles/sEmestralkaF3.dir/main.c.o: CMakeFiles/sEmestralkaF3.dir/flags.make
CMakeFiles/sEmestralkaF3.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.uYxTz0VSd9/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/sEmestralkaF3.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sEmestralkaF3.dir/main.c.o -c /tmp/tmp.uYxTz0VSd9/main.c

CMakeFiles/sEmestralkaF3.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sEmestralkaF3.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.uYxTz0VSd9/main.c > CMakeFiles/sEmestralkaF3.dir/main.c.i

CMakeFiles/sEmestralkaF3.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sEmestralkaF3.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.uYxTz0VSd9/main.c -o CMakeFiles/sEmestralkaF3.dir/main.c.s

# Object files for target sEmestralkaF3
sEmestralkaF3_OBJECTS = \
"CMakeFiles/sEmestralkaF3.dir/main.c.o"

# External object files for target sEmestralkaF3
sEmestralkaF3_EXTERNAL_OBJECTS =

sEmestralkaF3: CMakeFiles/sEmestralkaF3.dir/main.c.o
sEmestralkaF3: CMakeFiles/sEmestralkaF3.dir/build.make
sEmestralkaF3: CMakeFiles/sEmestralkaF3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.uYxTz0VSd9/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable sEmestralkaF3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sEmestralkaF3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sEmestralkaF3.dir/build: sEmestralkaF3

.PHONY : CMakeFiles/sEmestralkaF3.dir/build

CMakeFiles/sEmestralkaF3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sEmestralkaF3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sEmestralkaF3.dir/clean

CMakeFiles/sEmestralkaF3.dir/depend:
	cd /tmp/tmp.uYxTz0VSd9/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.uYxTz0VSd9 /tmp/tmp.uYxTz0VSd9 /tmp/tmp.uYxTz0VSd9/cmake-build-debug /tmp/tmp.uYxTz0VSd9/cmake-build-debug /tmp/tmp.uYxTz0VSd9/cmake-build-debug/CMakeFiles/sEmestralkaF3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sEmestralkaF3.dir/depend

