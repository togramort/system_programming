# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\prog\CLion 2020.2.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\prog\CLion 2020.2.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\program etc\projects2\sp\ipc2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\program etc\projects2\sp\ipc2\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/ipc2.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/ipc2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ipc2.dir/flags.make

CMakeFiles/ipc2.dir/processB.c.obj: CMakeFiles/ipc2.dir/flags.make
CMakeFiles/ipc2.dir/processB.c.obj: ../processB.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\program etc\projects2\sp\ipc2\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ipc2.dir/processB.c.obj"
	D:\prog\mingw\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\ipc2.dir\processB.c.obj -c "D:\program etc\projects2\sp\ipc2\processB.c"

CMakeFiles/ipc2.dir/processB.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ipc2.dir/processB.c.i"
	D:\prog\mingw\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "D:\program etc\projects2\sp\ipc2\processB.c" > CMakeFiles\ipc2.dir\processB.c.i

CMakeFiles/ipc2.dir/processB.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ipc2.dir/processB.c.s"
	D:\prog\mingw\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "D:\program etc\projects2\sp\ipc2\processB.c" -o CMakeFiles\ipc2.dir\processB.c.s

# Object files for target ipc2
ipc2_OBJECTS = \
"CMakeFiles/ipc2.dir/processB.c.obj"

# External object files for target ipc2
ipc2_EXTERNAL_OBJECTS =

ipc2.exe: CMakeFiles/ipc2.dir/processB.c.obj
ipc2.exe: CMakeFiles/ipc2.dir/build.make
ipc2.exe: CMakeFiles/ipc2.dir/linklibs.rsp
ipc2.exe: CMakeFiles/ipc2.dir/objects1.rsp
ipc2.exe: CMakeFiles/ipc2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\program etc\projects2\sp\ipc2\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ipc2.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\ipc2.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ipc2.dir/build: ipc2.exe
.PHONY : CMakeFiles/ipc2.dir/build

CMakeFiles/ipc2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\ipc2.dir\cmake_clean.cmake
.PHONY : CMakeFiles/ipc2.dir/clean

CMakeFiles/ipc2.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\program etc\projects2\sp\ipc2" "D:\program etc\projects2\sp\ipc2" "D:\program etc\projects2\sp\ipc2\cmake-build-debug" "D:\program etc\projects2\sp\ipc2\cmake-build-debug" "D:\program etc\projects2\sp\ipc2\cmake-build-debug\CMakeFiles\ipc2.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/ipc2.dir/depend

