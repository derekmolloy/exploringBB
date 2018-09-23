#include <Wire.h>                  // Uses the Two-Wire Interface (TWI)

const byte slaveAddr = 0x44;       // the slave address of the Arduino
int registerAddr;                  // the shared register addr variable
 
void setup() {                     // the setup function -- called once
  TWBR=100000L;                    // the i2c clk freq: 400000L = 400kHz
  Wire.begin(slaveAddr);           // set Arduino as an I2C slave device
  Wire.onReceive(receiveRegister); // register receive listener below
  Wire.onRequest(respondData);     // register respond listener below
}

void loop() {                       
  delay(1000);                     // loop each second -- reduce load
}                     
 
void receiveRegister(int x){       // handler called when data available
  registerAddr = Wire.read();      // read in one-byte address from PB
}
 
void respondData(){                // handler that is called on response
  Wire.write(registerAddr);        // i.e., send the data back to the PB
}
