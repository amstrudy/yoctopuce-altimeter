# yoctopuce-altimeter
A ROS package for the yocto altimeter

## Setup

To use this ROS package, you must install the C++ API from Yoctopuce. Use the following link to download to your preferred location, and then use the setup script to build.

http://www.yoctopuce.com/EN/libraries.php

Then, install this ROS package. To run:

Make sure to source ROS:
`source /opt/ros/kinetic/setup.bash`

Then, to make sure the yocto API is accessible during runtime:
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/yocto/cpp/api/Binaries/linux/64bits
