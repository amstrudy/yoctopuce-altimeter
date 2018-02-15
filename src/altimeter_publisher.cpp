#include <stdlib.h>
#include <iostream>
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include "../include/yocto/yocto_api/yocto_altitude.h"
#include "../include/yocto/yocto_api/yocto_api.h"
#include "../include/yocto/yocto_api/yocto_genericsensor.h"
#include "../include/yocto/yocto_api/yocto_temperature.h"
#include <yoctopuce_altimeter/yocto_msg.h>

int main (int argc, char *argv[])
{
	ros::init(argc, argv, "altimeter_pub_node");

	ros::NodeHandle n;

	ros::Publisher raw_pub = n.advertise<yoctopuce_altimeter::yocto_msg>("/yocto/raw", 1000);
	ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("/yocto/odom", 1000);

	ros::Rate loop_rate(10);

	string errmsg, target;
    YAltitude *asensor;
	YTemperature *tsensor;

    target = "any";

    // Setup the API to use local USB devices
	if (yRegisterHub("usb", errmsg) != YAPI_SUCCESS)
	{
		std::cerr << "RegisterHub error: " << errmsg << std::endl;
		return 1;
	}

	if (target == "any")
	{
		asensor = yFirstAltitude();
		tsensor = yFirstTemperature();
		if (asensor == NULL)
		{
			ROS_ERROR_STREAM("No module connected (check USB cable)" << std::endl);
			return 1;
		}
	}
	else {
		asensor = yFindAltitude(target + ".altitude");
		tsensor = yFindTemperature(target + ".temperature");
	}

    while(ros::ok())
	{
		if(!asensor->isOnline() && !tsensor->isOnline()) {
			ROS_ERROR_STREAM("Module not connected (check identification and USB cable)");
			break;
		}

		float alt = asensor->get_currentValue(); // m
		float pressure = asensor->get_qnh(); // hPa
		float temp = tsensor->get_currentValue(); // C

		yoctopuce_altimeter::yocto_msg raw_msg; // how to import custom message??
		nav_msgs::Odometry odom;

		raw_msg.header.frame_id = "base_link";
		raw_msg.altitude = alt;
		raw_msg.pressure = pressure;
		raw_msg.temperature.header.frame_id = "base_link";
		raw_msg.temperature.temperature = temp;
		raw_msg.temperature.variance = 0;
		raw_pub.publish(raw_msg);

		odom.header.frame_id = "odom";
		odom.child_frame_id = "odom";
		odom.pose.pose.position.z = alt;
		odom_pub.publish(odom);

		ros::spinOnce();

		loop_rate.sleep();

		// Uncomment below to recieve debug information
		// ROS_INFO_STREAM("Current altitude: " << asensor->get_currentValue() << " m (QNH= " << asensor->get_qnh() << " hPa)" << std::endl);

		ySleep(1000, errmsg);
	}

	yFreeAPI();
	return 0;
}
