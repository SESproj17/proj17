# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/lizi/msgs/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lizi/msgs/build

# Utility rule file for multi_sync_generate_messages_cpp.

# Include the progress variables for this target.
include multi_sync/CMakeFiles/multi_sync_generate_messages_cpp.dir/progress.make

multi_sync/CMakeFiles/multi_sync_generate_messages_cpp: /home/lizi/msgs/devel/include/multi_sync/Path.h
multi_sync/CMakeFiles/multi_sync_generate_messages_cpp: /home/lizi/msgs/devel/include/multi_sync/step.h
multi_sync/CMakeFiles/multi_sync_generate_messages_cpp: /home/lizi/msgs/devel/include/multi_sync/RobotStatus.h

/home/lizi/msgs/devel/include/multi_sync/Path.h: /opt/ros/indigo/share/gencpp/cmake/../../../lib/gencpp/gen_cpp.py
/home/lizi/msgs/devel/include/multi_sync/Path.h: /home/lizi/msgs/src/multi_sync/msg/Path.msg
/home/lizi/msgs/devel/include/multi_sync/Path.h: /opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg
/home/lizi/msgs/devel/include/multi_sync/Path.h: /opt/ros/indigo/share/gencpp/cmake/../msg.h.template
	$(CMAKE_COMMAND) -E cmake_progress_report /home/lizi/msgs/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating C++ code from multi_sync/Path.msg"
	cd /home/lizi/msgs/build/multi_sync && ../catkin_generated/env_cached.sh /usr/bin/python /opt/ros/indigo/share/gencpp/cmake/../../../lib/gencpp/gen_cpp.py /home/lizi/msgs/src/multi_sync/msg/Path.msg -Imulti_sync:/home/lizi/msgs/src/multi_sync/msg -Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg -p multi_sync -o /home/lizi/msgs/devel/include/multi_sync -e /opt/ros/indigo/share/gencpp/cmake/..

/home/lizi/msgs/devel/include/multi_sync/step.h: /opt/ros/indigo/share/gencpp/cmake/../../../lib/gencpp/gen_cpp.py
/home/lizi/msgs/devel/include/multi_sync/step.h: /home/lizi/msgs/src/multi_sync/msg/step.msg
/home/lizi/msgs/devel/include/multi_sync/step.h: /opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg
/home/lizi/msgs/devel/include/multi_sync/step.h: /opt/ros/indigo/share/gencpp/cmake/../msg.h.template
	$(CMAKE_COMMAND) -E cmake_progress_report /home/lizi/msgs/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating C++ code from multi_sync/step.msg"
	cd /home/lizi/msgs/build/multi_sync && ../catkin_generated/env_cached.sh /usr/bin/python /opt/ros/indigo/share/gencpp/cmake/../../../lib/gencpp/gen_cpp.py /home/lizi/msgs/src/multi_sync/msg/step.msg -Imulti_sync:/home/lizi/msgs/src/multi_sync/msg -Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg -p multi_sync -o /home/lizi/msgs/devel/include/multi_sync -e /opt/ros/indigo/share/gencpp/cmake/..

/home/lizi/msgs/devel/include/multi_sync/RobotStatus.h: /opt/ros/indigo/share/gencpp/cmake/../../../lib/gencpp/gen_cpp.py
/home/lizi/msgs/devel/include/multi_sync/RobotStatus.h: /home/lizi/msgs/src/multi_sync/msg/RobotStatus.msg
/home/lizi/msgs/devel/include/multi_sync/RobotStatus.h: /opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg
/home/lizi/msgs/devel/include/multi_sync/RobotStatus.h: /opt/ros/indigo/share/gencpp/cmake/../msg.h.template
	$(CMAKE_COMMAND) -E cmake_progress_report /home/lizi/msgs/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating C++ code from multi_sync/RobotStatus.msg"
	cd /home/lizi/msgs/build/multi_sync && ../catkin_generated/env_cached.sh /usr/bin/python /opt/ros/indigo/share/gencpp/cmake/../../../lib/gencpp/gen_cpp.py /home/lizi/msgs/src/multi_sync/msg/RobotStatus.msg -Imulti_sync:/home/lizi/msgs/src/multi_sync/msg -Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg -p multi_sync -o /home/lizi/msgs/devel/include/multi_sync -e /opt/ros/indigo/share/gencpp/cmake/..

multi_sync_generate_messages_cpp: multi_sync/CMakeFiles/multi_sync_generate_messages_cpp
multi_sync_generate_messages_cpp: /home/lizi/msgs/devel/include/multi_sync/Path.h
multi_sync_generate_messages_cpp: /home/lizi/msgs/devel/include/multi_sync/step.h
multi_sync_generate_messages_cpp: /home/lizi/msgs/devel/include/multi_sync/RobotStatus.h
multi_sync_generate_messages_cpp: multi_sync/CMakeFiles/multi_sync_generate_messages_cpp.dir/build.make
.PHONY : multi_sync_generate_messages_cpp

# Rule to build all files generated by this target.
multi_sync/CMakeFiles/multi_sync_generate_messages_cpp.dir/build: multi_sync_generate_messages_cpp
.PHONY : multi_sync/CMakeFiles/multi_sync_generate_messages_cpp.dir/build

multi_sync/CMakeFiles/multi_sync_generate_messages_cpp.dir/clean:
	cd /home/lizi/msgs/build/multi_sync && $(CMAKE_COMMAND) -P CMakeFiles/multi_sync_generate_messages_cpp.dir/cmake_clean.cmake
.PHONY : multi_sync/CMakeFiles/multi_sync_generate_messages_cpp.dir/clean

multi_sync/CMakeFiles/multi_sync_generate_messages_cpp.dir/depend:
	cd /home/lizi/msgs/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lizi/msgs/src /home/lizi/msgs/src/multi_sync /home/lizi/msgs/build /home/lizi/msgs/build/multi_sync /home/lizi/msgs/build/multi_sync/CMakeFiles/multi_sync_generate_messages_cpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : multi_sync/CMakeFiles/multi_sync_generate_messages_cpp.dir/depend

