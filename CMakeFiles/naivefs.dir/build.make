# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/guests/190110611/user-land-filesystem/fs/dearfs

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/guests/190110611/user-land-filesystem/fs/dearfs

# Include any dependencies generated for this target.
include CMakeFiles/naivefs.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/naivefs.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/naivefs.dir/flags.make

CMakeFiles/naivefs.dir/src/naivefs.c.o: CMakeFiles/naivefs.dir/flags.make
CMakeFiles/naivefs.dir/src/naivefs.c.o: src/naivefs.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guests/190110611/user-land-filesystem/fs/dearfs/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/naivefs.dir/src/naivefs.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/naivefs.dir/src/naivefs.c.o   -c /home/guests/190110611/user-land-filesystem/fs/dearfs/src/naivefs.c

CMakeFiles/naivefs.dir/src/naivefs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/naivefs.dir/src/naivefs.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/guests/190110611/user-land-filesystem/fs/dearfs/src/naivefs.c > CMakeFiles/naivefs.dir/src/naivefs.c.i

CMakeFiles/naivefs.dir/src/naivefs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/naivefs.dir/src/naivefs.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/guests/190110611/user-land-filesystem/fs/dearfs/src/naivefs.c -o CMakeFiles/naivefs.dir/src/naivefs.c.s

CMakeFiles/naivefs.dir/src/naivefs_driver.c.o: CMakeFiles/naivefs.dir/flags.make
CMakeFiles/naivefs.dir/src/naivefs_driver.c.o: src/naivefs_driver.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guests/190110611/user-land-filesystem/fs/dearfs/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/naivefs.dir/src/naivefs_driver.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/naivefs.dir/src/naivefs_driver.c.o   -c /home/guests/190110611/user-land-filesystem/fs/dearfs/src/naivefs_driver.c

CMakeFiles/naivefs.dir/src/naivefs_driver.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/naivefs.dir/src/naivefs_driver.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/guests/190110611/user-land-filesystem/fs/dearfs/src/naivefs_driver.c > CMakeFiles/naivefs.dir/src/naivefs_driver.c.i

CMakeFiles/naivefs.dir/src/naivefs_driver.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/naivefs.dir/src/naivefs_driver.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/guests/190110611/user-land-filesystem/fs/dearfs/src/naivefs_driver.c -o CMakeFiles/naivefs.dir/src/naivefs_driver.c.s

CMakeFiles/naivefs.dir/src/naivefs_funct.c.o: CMakeFiles/naivefs.dir/flags.make
CMakeFiles/naivefs.dir/src/naivefs_funct.c.o: src/naivefs_funct.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guests/190110611/user-land-filesystem/fs/dearfs/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/naivefs.dir/src/naivefs_funct.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/naivefs.dir/src/naivefs_funct.c.o   -c /home/guests/190110611/user-land-filesystem/fs/dearfs/src/naivefs_funct.c

CMakeFiles/naivefs.dir/src/naivefs_funct.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/naivefs.dir/src/naivefs_funct.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/guests/190110611/user-land-filesystem/fs/dearfs/src/naivefs_funct.c > CMakeFiles/naivefs.dir/src/naivefs_funct.c.i

CMakeFiles/naivefs.dir/src/naivefs_funct.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/naivefs.dir/src/naivefs_funct.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/guests/190110611/user-land-filesystem/fs/dearfs/src/naivefs_funct.c -o CMakeFiles/naivefs.dir/src/naivefs_funct.c.s

CMakeFiles/naivefs.dir/src/naivefs_struct.c.o: CMakeFiles/naivefs.dir/flags.make
CMakeFiles/naivefs.dir/src/naivefs_struct.c.o: src/naivefs_struct.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guests/190110611/user-land-filesystem/fs/dearfs/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/naivefs.dir/src/naivefs_struct.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/naivefs.dir/src/naivefs_struct.c.o   -c /home/guests/190110611/user-land-filesystem/fs/dearfs/src/naivefs_struct.c

CMakeFiles/naivefs.dir/src/naivefs_struct.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/naivefs.dir/src/naivefs_struct.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/guests/190110611/user-land-filesystem/fs/dearfs/src/naivefs_struct.c > CMakeFiles/naivefs.dir/src/naivefs_struct.c.i

CMakeFiles/naivefs.dir/src/naivefs_struct.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/naivefs.dir/src/naivefs_struct.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/guests/190110611/user-land-filesystem/fs/dearfs/src/naivefs_struct.c -o CMakeFiles/naivefs.dir/src/naivefs_struct.c.s

# Object files for target naivefs
naivefs_OBJECTS = \
"CMakeFiles/naivefs.dir/src/naivefs.c.o" \
"CMakeFiles/naivefs.dir/src/naivefs_driver.c.o" \
"CMakeFiles/naivefs.dir/src/naivefs_funct.c.o" \
"CMakeFiles/naivefs.dir/src/naivefs_struct.c.o"

# External object files for target naivefs
naivefs_EXTERNAL_OBJECTS =

naivefs: CMakeFiles/naivefs.dir/src/naivefs.c.o
naivefs: CMakeFiles/naivefs.dir/src/naivefs_driver.c.o
naivefs: CMakeFiles/naivefs.dir/src/naivefs_funct.c.o
naivefs: CMakeFiles/naivefs.dir/src/naivefs_struct.c.o
naivefs: CMakeFiles/naivefs.dir/build.make
naivefs: /usr/lib/x86_64-linux-gnu/libfuse.so
naivefs: /home/guests/190110611/lib/libddriver.a
naivefs: CMakeFiles/naivefs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/guests/190110611/user-land-filesystem/fs/dearfs/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable naivefs"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/naivefs.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/naivefs.dir/build: naivefs

.PHONY : CMakeFiles/naivefs.dir/build

CMakeFiles/naivefs.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/naivefs.dir/cmake_clean.cmake
.PHONY : CMakeFiles/naivefs.dir/clean

CMakeFiles/naivefs.dir/depend:
	cd /home/guests/190110611/user-land-filesystem/fs/dearfs && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/guests/190110611/user-land-filesystem/fs/dearfs /home/guests/190110611/user-land-filesystem/fs/dearfs /home/guests/190110611/user-land-filesystem/fs/dearfs /home/guests/190110611/user-land-filesystem/fs/dearfs /home/guests/190110611/user-land-filesystem/fs/dearfs/CMakeFiles/naivefs.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/naivefs.dir/depend

