# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/emre/.scripts/EncryptedSystemMonitor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/emre/.scripts/EncryptedSystemMonitor/build

# Include any dependencies generated for this target.
include CMakeFiles/monitor.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/monitor.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/monitor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/monitor.dir/flags.make

CMakeFiles/monitor.dir/src/monitor.cpp.o: CMakeFiles/monitor.dir/flags.make
CMakeFiles/monitor.dir/src/monitor.cpp.o: ../src/monitor.cpp
CMakeFiles/monitor.dir/src/monitor.cpp.o: CMakeFiles/monitor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emre/.scripts/EncryptedSystemMonitor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/monitor.dir/src/monitor.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/monitor.dir/src/monitor.cpp.o -MF CMakeFiles/monitor.dir/src/monitor.cpp.o.d -o CMakeFiles/monitor.dir/src/monitor.cpp.o -c /home/emre/.scripts/EncryptedSystemMonitor/src/monitor.cpp

CMakeFiles/monitor.dir/src/monitor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monitor.dir/src/monitor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/emre/.scripts/EncryptedSystemMonitor/src/monitor.cpp > CMakeFiles/monitor.dir/src/monitor.cpp.i

CMakeFiles/monitor.dir/src/monitor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monitor.dir/src/monitor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/emre/.scripts/EncryptedSystemMonitor/src/monitor.cpp -o CMakeFiles/monitor.dir/src/monitor.cpp.s

# Object files for target monitor
monitor_OBJECTS = \
"CMakeFiles/monitor.dir/src/monitor.cpp.o"

# External object files for target monitor
monitor_EXTERNAL_OBJECTS =

monitor: CMakeFiles/monitor.dir/src/monitor.cpp.o
monitor: CMakeFiles/monitor.dir/build.make
monitor: ../lib/local/poco/lib/libPocoUtil.so.103
monitor: ../lib/local/poco/lib/libPocoXML.so.103
monitor: ../lib/local/poco/lib/libPocoJSON.so.103
monitor: ../lib/local/poco/lib/libPocoFoundation.so.103
monitor: CMakeFiles/monitor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/emre/.scripts/EncryptedSystemMonitor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable monitor"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/monitor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/monitor.dir/build: monitor
.PHONY : CMakeFiles/monitor.dir/build

CMakeFiles/monitor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/monitor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/monitor.dir/clean

CMakeFiles/monitor.dir/depend:
	cd /home/emre/.scripts/EncryptedSystemMonitor/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/emre/.scripts/EncryptedSystemMonitor /home/emre/.scripts/EncryptedSystemMonitor /home/emre/.scripts/EncryptedSystemMonitor/build /home/emre/.scripts/EncryptedSystemMonitor/build /home/emre/.scripts/EncryptedSystemMonitor/build/CMakeFiles/monitor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/monitor.dir/depend

