#include<iostream>
#include<unistd.h>
#include<iomanip>
#include<bcm2835.h>
using namespace std;

int main() {
   if (!bcm2835_init()){
      cout << "Failed exiting " << endl;
      return 1;
   }
   cout << "Using the BCM2835 I2C code"  << endl;

   bcm2835_i2c_begin();

   uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_148;
   bcm2835_i2c_setSlaveAddress(0x44);
//   bcm2835_i2c_setClockDivider(clk_div);
   cout << "Clock divider set to: %d\n" <<  clk_div << endl;

   char buf[1],  wbuf[1] = {0x03};
   uint8_t data = bcm2835_i2c_write(wbuf, 1);
   data = bcm2835_i2c_read(buf, 1);

   cout << "The value of the read data is:" << (int) buf[0] << endl;
   bcm2835_i2c_end();

   bcm2835_close();
   return 0;
}
