ros_polhemus_fastrak
====================
Unfinished

This node publishes fastrack(polhemus) pose data as geometry_msgs/PoseStamped on ROS
and broadcasts the pose as "/fastrack" from "/world" on TF.


This node access the fastrack via Serial port and has only read function,
so you need to initialise and set the parameters of the fastrack using other serial terminal.
