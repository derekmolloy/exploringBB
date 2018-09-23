#include <Wire.h>                  // uses the Two-Wire Interface (TWI)
const byte slaveAddr = 0x44;       // the slave address of the Arduino
int registerAddr;                  // the shared register addr variable
const int analogInPin = A0;        // analog input for the TMP36 
int data[4];                       // the data registers 00 to 0x03
 
void setup(){
  TWBR=100000L;                    // set the i2c clk freq e.g. 100000L
  Wire.begin(slaveAddr);           // set up the Arduino as an I2C slave
  Wire.onReceive(receiveRegister); // register receive listener below
  Wire.onRequest(respondData);     // register respond listener below
}

void loop(){                       // update registers every five seconds
  int adcValue = analogRead(analogInPin);           // using a 10-bit ADC
  float curVoltage = adcValue * (5.0f/1024.0f);     // Vcc = 5.0V, 10-bit
  float tempC = 25.0 + ((curVoltage-0.75f)/0.01f);  // from datasheet
  float tempF = 32.0 + ((tempC * 9)/5);             // deg. C to F
  data[0] = (int) tempC;                            // whole deg C (0x00)
  data[1] = (int) ((tempC - data[0])*100);          // fract C     (0x01)
  data[2] = (int) tempF;                            // whole deg F (0x02)
  data[3] = (int) ((tempF - data[2])*100);          // fract F     (0x03)
  delay(5000);                                      // delay 5 seconds
}                     
 
void receiveRegister(int x){       // passes the number of bytes
  registerAddr = Wire.read();      // read in the one-byte address
}
 
void respondData(){                // respond function
  byte dataValue = 0x00;           // default response value is 0x00
  if ((registerAddr >= 0x00) && (registerAddr <0x04)){      
     dataValue = data[registerAddr];
  }
  Wire.write(dataValue);           // send the data back to the PB
}
