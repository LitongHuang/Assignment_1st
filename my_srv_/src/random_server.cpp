#include "ros/ros.h"
#include "my_srv_/Random.h"
#include<math.h>

// Random numbers
double randMToN(double M, double N)
{     return M + (rand() / ( RAND_MAX / (N-M) ) ) ; }


bool myrandom (my_srv_::Random::Request &req, my_srv_::Random::Response &res){
	res.x = randMToN(req.min, req.max);
    res.y = randMToN(req.min, req.max);
    return true;
}



int main(int argc, char **argv)
{
   //initialize ros
   ros::init(argc, argv, "random_server");
   ros::NodeHandle n;
   //Server replying to the node /assignment1
   ros::ServiceServer service= n.advertiseService("/random", myrandom);
   ros::spin();

   return 0;
}








