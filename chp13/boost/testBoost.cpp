/* A program to test the Boost library on the BeagleBone
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
using namespace boost::geometry::model::d2;
#include <iostream>

int main(){
   point_xy<float> p1(1.0,2.0), p2(3.0,4.0);
   float distance = boost::geometry::distance(p1,p2);
   std::cout << "Distance between points is: " << distance << std::endl;
   return 0;
}

