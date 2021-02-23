#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "assignment1/Vel.h" //Custome messages
#include "my_srv_/Random.h"
#include <sstream>
#include <iostream>

//Target positions
double xt = 0, yt = 0;
int k = 2;

//Declare the publisher and service
ros::Publisher pub;
ros::Publisher pub2;
ros::ServiceClient client2;


void positionCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
  ROS_INFO("Robot position [%f, %f]", msg->pose.pose.position.x, msg->pose.pose.position.y);
  geometry_msgs::Twist vel;
  my_srv_::Random reach_target;
  

  //Distance between target and robot position:
  double dx = xt-msg->pose.pose.position.x;
  double dy = yt-msg->pose.pose.position.y;
  
  //Reaching the target if the distance is less than 0.1
  if (sqrt(dx) + sqrt(dy) <= sqrt(0.1)) {
     reach_target.request.min = -6;
     reach_target.request.max = 6;
     client2.call(reach_target);
     
     xt = reach_target.response.x;
     yt = reach_target.response.y;
     
     dx = xt-msg->pose.pose.position.x;
     dy = yt-msg->pose.pose.position.y;
  }  
  //Velocity of the robot
  vel.linear.x = k*dx;
  vel.linear.y = k*dy;
  pub.publish(vel);

  assignment1::Vel mymsg;
  mymsg.name = "linear";
  mymsg.Vx = vel.linear.x;
  mymsg.Vy = vel.linear.y;
  pub2.publish(mymsg);
  
}

int main(int argc, char **argv)
{
  //initialize
  ros::init(argc, argv, "assignment1");
  ros::NodeHandle n;

  client2 = n.serviceClient<my_srv_::Random>("/random");

  pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  pub2 = n.advertise<assignment1::Vel>("linear", 1000);

  ros::Subscriber sub = n.subscribe("odom", 1000, positionCallback);

  ros::spin();

  return 0;
}
