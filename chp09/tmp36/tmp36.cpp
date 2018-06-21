/* Using the tmp36 sensor with the MCP3208 */
#include <iostream>
#include "bus/SPIDevice.h"
using namespace exploringBB;
using namespace std;

float getTemperature(int adc_value){     // from the TMP36 datasheet
   float cur_voltage = adc_value * (3.30f/4096.0f); // Vcc = 3.3V, 12-bit
   float diff_degreesC = (cur_voltage-0.75f)/0.01f;
   return (25.0f + diff_degreesC);
}

int main(){
   cout << "Starting the TMP36 example" << endl;
   SPIDevice *busDevice = new SPIDevice(0,0);
   busDevice->setSpeed(1000000);         // max frequency limit at 3.3V
   busDevice->setMode(SPIDevice::MODE0);
   unsigned char send[3], receive[3];
   send[0] = 0b00000110;     // Reading single-ended input from channel 0
   send[1] = 0b00000000;     // Use 0b00000001 and 0b10000000 for MCP3008
   busDevice->transfer(send, receive, 3);
   float temp = getTemperature(((receive[1]&0b00001111)<<8)|receive[2]);
   float fahr = 32 + ((temp * 9)/5);    // convert deg. C to deg. F
   cout << "Temperature is " << temp << " °C (" << fahr << "°F)" << endl;
   busDevice->close();
   return 0;
}
