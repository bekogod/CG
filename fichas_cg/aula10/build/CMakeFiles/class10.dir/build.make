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
CMAKE_SOURCE_DIR = /home/beko/Desktop/UNI/CG/fichas_cg/aula10

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/beko/Desktop/UNI/CG/fichas_cg/aula10/build

# Include any dependencies generated for this target.
include CMakeFiles/class10.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/class10.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/class10.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/class10.dir/flags.make

CMakeFiles/class10.dir/main.cpp.o: CMakeFiles/class10.dir/flags.make
CMakeFiles/class10.dir/main.cpp.o: ../main.cpp
CMakeFiles/class10.dir/main.cpp.o: CMakeFiles/class10.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/beko/Desktop/UNI/CG/fichas_cg/aula10/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/class10.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/class10.dir/main.cpp.o -MF CMakeFiles/class10.dir/main.cpp.o.d -o CMakeFiles/class10.dir/main.cpp.o -c /home/beko/Desktop/UNI/CG/fichas_cg/aula10/main.cpp

CMakeFiles/class10.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/class10.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/beko/Desktop/UNI/CG/fichas_cg/aula10/main.cpp > CMakeFiles/class10.dir/main.cpp.i

CMakeFiles/class10.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/class10.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/beko/Desktop/UNI/CG/fichas_cg/aula10/main.cpp -o CMakeFiles/class10.dir/main.cpp.s

# Object files for target class10
class10_OBJECTS = \
"CMakeFiles/class10.dir/main.cpp.o"

# External object files for target class10
class10_EXTERNAL_OBJECTS =

class10: CMakeFiles/class10.dir/main.cpp.o
class10: CMakeFiles/class10.dir/build.make
class10: /usr/lib/x86_64-linux-gnu/libGL.so
class10: /usr/lib/x86_64-linux-gnu/libGLU.so
class10: /usr/lib/x86_64-linux-gnu/libglut.so
class10: /usr/lib/x86_64-linux-gnu/libXmu.so
class10: /usr/lib/x86_64-linux-gnu/libXi.so
class10: /usr/lib/x86_64-linux-gnu/libGLEW.so
class10: /usr/lib/x86_64-linux-gnu/libIL.so
class10: CMakeFiles/class10.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/beko/Desktop/UNI/CG/fichas_cg/aula10/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable class10"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/class10.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/class10.dir/build: class10
.PHONY : CMakeFiles/class10.dir/build

CMakeFiles/class10.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/class10.dir/cmake_clean.cmake
.PHONY : CMakeFiles/class10.dir/clean

CMakeFiles/class10.dir/depend:
	cd /home/beko/Desktop/UNI/CG/fichas_cg/aula10/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/beko/Desktop/UNI/CG/fichas_cg/aula10 /home/beko/Desktop/UNI/CG/fichas_cg/aula10 /home/beko/Desktop/UNI/CG/fichas_cg/aula10/build /home/beko/Desktop/UNI/CG/fichas_cg/aula10/build /home/beko/Desktop/UNI/CG/fichas_cg/aula10/build/CMakeFiles/class10.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/class10.dir/depend

