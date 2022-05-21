# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /root/wokespace/master_rpc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/wokespace/master_rpc/build

# Include any dependencies generated for this target.
include CMakeFiles/master_client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/master_client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/master_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/master_client.dir/flags.make

CMakeFiles/master_client.dir/Client/Master_client.cpp.o: CMakeFiles/master_client.dir/flags.make
CMakeFiles/master_client.dir/Client/Master_client.cpp.o: ../Client/Master_client.cpp
CMakeFiles/master_client.dir/Client/Master_client.cpp.o: CMakeFiles/master_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/wokespace/master_rpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/master_client.dir/Client/Master_client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/master_client.dir/Client/Master_client.cpp.o -MF CMakeFiles/master_client.dir/Client/Master_client.cpp.o.d -o CMakeFiles/master_client.dir/Client/Master_client.cpp.o -c /root/wokespace/master_rpc/Client/Master_client.cpp

CMakeFiles/master_client.dir/Client/Master_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/master_client.dir/Client/Master_client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/wokespace/master_rpc/Client/Master_client.cpp > CMakeFiles/master_client.dir/Client/Master_client.cpp.i

CMakeFiles/master_client.dir/Client/Master_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/master_client.dir/Client/Master_client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/wokespace/master_rpc/Client/Master_client.cpp -o CMakeFiles/master_client.dir/Client/Master_client.cpp.s

# Object files for target master_client
master_client_OBJECTS = \
"CMakeFiles/master_client.dir/Client/Master_client.cpp.o"

# External object files for target master_client
master_client_EXTERNAL_OBJECTS =

../bin/master_client: CMakeFiles/master_client.dir/Client/Master_client.cpp.o
../bin/master_client: CMakeFiles/master_client.dir/build.make
../bin/master_client: ../lib/librpc.so
../bin/master_client: CMakeFiles/master_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/wokespace/master_rpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/master_client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/master_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/master_client.dir/build: ../bin/master_client
.PHONY : CMakeFiles/master_client.dir/build

CMakeFiles/master_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/master_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/master_client.dir/clean

CMakeFiles/master_client.dir/depend:
	cd /root/wokespace/master_rpc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/wokespace/master_rpc /root/wokespace/master_rpc /root/wokespace/master_rpc/build /root/wokespace/master_rpc/build /root/wokespace/master_rpc/build/CMakeFiles/master_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/master_client.dir/depend

