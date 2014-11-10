ros_polhemus_fastrak
====================
Unfinished

This node publishes fastrack(polhemus) pose data as geometry_msgs/PoseStamped on ROS
and broadcasts the poses as "/fastrack" from "/world" on TF.


This node access the fastrack via Serial port and provcides only read function,
so you need to initialise and set the parameters of the fastrack using other serial terminal software.

## License

Copyright (c) 2014, [Naohiro Hayashi](http://rinrin-mizutama.org). (MIT License)

See LICENSE for more info.
