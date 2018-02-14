cmake_minimum_required(VERSION 2.8.3)
project(yoctopuce_altimeter)

find_package(
		catkin REQUIRED COMPONENTS
		nav_msgs
		roscpp
		std_msgs
		sensor_msgs
		message_generation
)

add_message_files(
		DIRECTORY msg
		FILES yocto_msg.msg
)

generate_messages(
		DEPENDENCIES
		nav_msgs
		std_msgs
		sensor_msgs
 )

catkin_package(
		LIBRARIES altimeter_pub
		CATKIN_DEPENDS nav_msgs roscpp std_msgs sensor_msgs message_runtime
)

include_directories(
	${catkin_INCLUDE_DIRS}
)

add_executable(
		${PROJECT_NAME}
		src/altimeter_publisher.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/include/yocto/64bits/libyocto-static.a)

target_link_libraries(
		${PROJECT_NAME} 
		${catkin_LIBRARIES} 
		${CMAKE_CURRENT_SOURCE_DIR}/include/yocto/64bits/libyocto.so.1.0.1
		)
