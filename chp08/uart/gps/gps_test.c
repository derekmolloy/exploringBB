#include<stdio.h>
#include<stdlib.h>
#include<gps.h>

int main(){
   gps_init();                   // initialize the device
   loc_t gps;                    // a location structure
   gps_location(&gps);           // determine the location data
   printf("The board location is (%lf,%lf)\n", gps.latitude, gps.longitude);
   printf("Altitude: %lf m. Speed: %lf knots\n", gps.altitude, gps.speed);
   return 0;
}
