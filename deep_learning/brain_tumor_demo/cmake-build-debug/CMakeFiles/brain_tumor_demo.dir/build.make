# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /home/thomas/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/183.5429.37/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/thomas/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/183.5429.37/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/brain_tumor_demo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/brain_tumor_demo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/brain_tumor_demo.dir/flags.make

CMakeFiles/brain_tumor_demo.dir/brain_image_factory.c.o: CMakeFiles/brain_tumor_demo.dir/flags.make
CMakeFiles/brain_tumor_demo.dir/brain_image_factory.c.o: ../brain_image_factory.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/brain_tumor_demo.dir/brain_image_factory.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/brain_tumor_demo.dir/brain_image_factory.c.o   -c /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/brain_image_factory.c

CMakeFiles/brain_tumor_demo.dir/brain_image_factory.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/brain_tumor_demo.dir/brain_image_factory.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/brain_image_factory.c > CMakeFiles/brain_tumor_demo.dir/brain_image_factory.c.i

CMakeFiles/brain_tumor_demo.dir/brain_image_factory.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/brain_tumor_demo.dir/brain_image_factory.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/brain_image_factory.c -o CMakeFiles/brain_tumor_demo.dir/brain_image_factory.c.s

CMakeFiles/brain_tumor_demo.dir/image_utils.c.o: CMakeFiles/brain_tumor_demo.dir/flags.make
CMakeFiles/brain_tumor_demo.dir/image_utils.c.o: ../image_utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/brain_tumor_demo.dir/image_utils.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/brain_tumor_demo.dir/image_utils.c.o   -c /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/image_utils.c

CMakeFiles/brain_tumor_demo.dir/image_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/brain_tumor_demo.dir/image_utils.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/image_utils.c > CMakeFiles/brain_tumor_demo.dir/image_utils.c.i

CMakeFiles/brain_tumor_demo.dir/image_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/brain_tumor_demo.dir/image_utils.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/image_utils.c -o CMakeFiles/brain_tumor_demo.dir/image_utils.c.s

# Object files for target brain_tumor_demo
brain_tumor_demo_OBJECTS = \
"CMakeFiles/brain_tumor_demo.dir/brain_image_factory.c.o" \
"CMakeFiles/brain_tumor_demo.dir/image_utils.c.o"

# External object files for target brain_tumor_demo
brain_tumor_demo_EXTERNAL_OBJECTS =

brain_tumor_demo: CMakeFiles/brain_tumor_demo.dir/brain_image_factory.c.o
brain_tumor_demo: CMakeFiles/brain_tumor_demo.dir/image_utils.c.o
brain_tumor_demo: CMakeFiles/brain_tumor_demo.dir/build.make
brain_tumor_demo: CMakeFiles/brain_tumor_demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable brain_tumor_demo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/brain_tumor_demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/brain_tumor_demo.dir/build: brain_tumor_demo

.PHONY : CMakeFiles/brain_tumor_demo.dir/build

CMakeFiles/brain_tumor_demo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/brain_tumor_demo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/brain_tumor_demo.dir/clean

CMakeFiles/brain_tumor_demo.dir/depend:
	cd /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/cmake-build-debug /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/cmake-build-debug /home/thomas/Drive/Thomas/cours/cours_2018_2019/machine_learning/deep_learning/brain_tumor_demo/cmake-build-debug/CMakeFiles/brain_tumor_demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/brain_tumor_demo.dir/depend
