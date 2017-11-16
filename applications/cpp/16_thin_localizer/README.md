# thin_localizer #

Particle Filter based localizer

## Prerequisites

This package needs [ROS kinetic (Ubuntu 16.04)](http://wiki.ros.org/kinetic/Installation/Ubuntu) or [ROS indigo (Ubuntu 14.04)](http://wiki.ros.org/indigo/Installation/Ubuntu)

## Compilation

Compile it with catkin.

	cd <catkin_workspace>/src
	ln -s <path-to-prob-rob-16-17>/applications/cpp/16_thin_localizer .
	cd ..
	catkin_make

## Test in simulation
Launch the simulator from the test directory

       rosrun stage_ros stageros dis-B1-2011-09-27.world

Launch the localizer, providing the map image

       rosrun thin_localizer thin_localizer_node dis-B1-2011-09-27.png

This will open a window showing the state of the localizer
Move the robot with a joystick or some other teleoperation means, e.g.

     sudo apt-get install ros-kinetic-teleop-twist-keyboard
     rosrun teleop_twist_keyboard teleop_twist_keyboard.py

## Test with real data
Reproduce the ROS bagfile contained in the folder test_real using ```rosbag```. Remember to launch first the ROS environment using ```roscore```.

     rosbag play dis-underground.bag

While the bag is running you can visualize information using ```rviz``` or by doing ```rostopic echo <topic>```

Launch the localizer, providing the image of the map contained in the folder test_real.

       rosrun thin_localizer thin_localizer_node dis_underground.pgm

Press the key 'g' on the GUI that will appear to perform a global localization call.
