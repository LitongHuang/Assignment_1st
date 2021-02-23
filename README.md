# Research Track I - first assignment
Name: Litong Huang 
Student ID: 5058374

### Description of the assignmnet
This assignment requires controlling a holonomic robot in a 2d space with a simple 2d simulator, Stage. 

Two ROS nodes are developed while the node `/stageros` is provided. The first node is `/assignment1`, it can be used to keep the position information of the target. This node calls a server for receiveing a random target, as well as making the robot reach the target if the distance between the target and robot is less than 0.1. A ROS publisher is used to set robot velocity (`/cmd_vel`). A ROS subscriber is used to find the actual robot position (`/odom`). The other node `/random_server` is acting as the service server, which it sets the random target and replies to the client.

### Computational graph of the system (`rqt_graph`)

![the graph of system](rosgraph.png)



### Instructions about how to run to code

First of all, the first thing you should do when using ROS is to execute the command (master):

```
roscore &
```

Then, use `rosnode` to display the ROS nodes that are currently running:
```
rosnode list

rosnode info /[node_name]
rosnode info /assignment1
rosnode info /random
```

Next, the simulator can be launched by executing the command:

```
rosrun stage_ros stageros $(rospack find assignment1)/world/exercise.world
```

Use `rosrun` to run a node directly:
```
rosrun [package_name] [node_name]
rosrun assignment1 assignment1
rosrun my_srv_ random
```






