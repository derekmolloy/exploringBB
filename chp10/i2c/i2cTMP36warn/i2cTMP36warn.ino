#include <Wire.h>                  // uses the Two-Wire Interface (TWI)
const byte slaveAddr = 0x44;       // the slave address of the Arduino
int registerAddr;                  // the shared register address variable
const int analogInPin = A0;        // analog input pin for the TMP36
int data[5];                       // the data registers 00 to 0x04
int alertTemp = 0xFF;              // alert temperature not set by default
int ledPin = 11;                   // the warning light LED
 
void setup(){
  pinMode(ledPin, OUTPUT);         // LED provides a visible temp alert
  TWBR=100000L;                    // set the i2c clk freq e.g. 100000L
  Wire.begin(slaveAddr);           // set up the Arduino as an I2C slave
  Wire.onReceive(receiveRegister); // register receive listener below
  Wire.onRequest(respondData);     // register respond listener below*/
  //Serial.begin(115200, SERIAL_8N1);  // remove for debug
}

void loop(){                       // update registers every five seconds
  int adcValue = analogRead(analogInPin);           // using a 10-bit ADC
  //Serial.print("\nThe ADC value is: ");           // remove for debug
  //Serial.print(adcValue);                         // remove for debug
  float curVoltage = adcValue * (3.3f/1024.0f);     // Vcc = 3.3V, 10-bit
  float tempC = 25.0 + ((curVoltage-0.75f)/0.01f);  // from datasheet
  float tempF = 32.0 + ((tempC * 9)/5);             // deg. C to deg. F
  data[0] = (int) tempC;                            // whole deg C (0x00)
  data[1] = (int) ((tempC - data[0])*100);          // fract deg C (0x01)
  data[2] = (int) tempF;                            // whole deg F (0x02)
  data[3] = (int) ((tempF - data[2])*100);          // fract deg F (0x03)
  data[4] = alertTemp;                              // alert tmp C (0x04)
  if (alertTemp <= tempC) {                         // test alert?
     digitalWrite(ledPin, HIGH);                    // yes, set LED on
  }
  else {
     digitalWrite(ledPin, LOW);                     // else LED off
  }
  delay(5000);
}                     
 
void receiveRegister(int x){       // passes the number of bytes
  registerAddr = Wire.read();      // read in the one-byte address
  if(registerAddr==0x04 && x==2){  // if writing the alert value
    alertTemp = Wire.read();       // read in the alert temperature
  }
}
 
void respondData(){                // respond function
  byte dataValue = 0x00;           // default response value is 0x00
  if ((registerAddr >= 0x00) && (registerAddr <= 0x04)){      
     dataValue = data[registerAddr];
  }
  Wire.write(dataValue);           // send the data back to the PB
}
