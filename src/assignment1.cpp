#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "assignment1/Vel.h" //Custome messages
#include "my_srv_/Random.h"
#include <sstream>
#include <iostream>


//Declare the publisher and service
ros::Publisher pub;
ros::Publisher pub2;
ros::ServiceClient client2;

//Target positions
double xt = 0, yt = 0, k = 2;


void positionCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
  ROS_INFO("Robot position [%f, %f]", msg->pose.pose.position.x, msg->pose.pose.position.y);

  my_srv_::Random reach_target;
  geometry_msgs::Twist vel;
  

  //Distance between target and robot position:
  double dx = xt - msg->pose.pose.position.x;
  double dy = yt - msg->pose.pose.position.y;
  
  //Reaching the target if the distance is less than 0.1
  if ((dx*dx) + (dy*dy) <= (0.1*0.1)) {

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
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "assignment1");
   /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  client2 = n.serviceClient<my_srv_::Random>("/random");

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */
  pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  pub2 = n.advertise<assignment1::Vel>("linear", 1000);
  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  ros::Subscriber sub = n.subscribe("odom", 1000, positionCallback);

  ros::spin();

  return 0;
}
