/* A program to test the ADXL345 client application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include "network/SocketClient.h"               // using the EBB library
#include "bus/I2CDevice.h"                      // I2CDevice class see CHP8
#include "sensor/ADXL345.h"                     // ADXL345 see CHP8
#include "sstream"                              // to format the string
#include <unistd.h>                             // for the usleep()
using namespace std;
using namespace exploringBB;

int main(int argc, char *argv[]){
   if(argc!=2){
      cout << "Usage:  accClient server_name" << endl;
      return 2;
   }
   cout << "Starting EBB ADXL345 Client Example" << endl;
   I2CDevice i2c(1,0x53);                       // the I2C device P9_19 P9_20
   ADXL345 sensor(&i2c);                        // pass device to ADXL const.
   sensor.setResolution(ADXL345::NORMAL);       // regular resolution
   sensor.setRange(ADXL345::PLUSMINUS_4_G);     // regular +/- 2G

   SocketClient sc(argv[1], 4444);              // server addr and port number
   sc.connectToServer();                        // connect to the server
   for(int i=0; i<1000; i++){                   // going to send 1000 samples
      stringstream ss;                          // use a stringstream for msg.
      sensor.readSensorState();                 // update the sensor state
      float pitch = sensor.getPitch();          // get pitch and roll
      float roll = sensor.getRoll();            // structure as XML string
      ss << "<sample><acc><pitch>" << pitch << "</pitch>";
      ss << "<roll>" << roll << "</roll></acc></sample>      ";
      cout << ss.str() << '\xd';                // print to output on one line
      cout.flush();                             // flush to update the display
      sc.send(ss.str());                        // send the same str to server
      usleep(50000);                            //50ms between samples
   }
   cout << "End of EBB Client Example" << endl;
}
