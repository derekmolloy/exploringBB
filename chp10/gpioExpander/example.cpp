/**
 * testApp.cpp This is a test program that uses the gpioExpander class to
 * utilize a MCP23017 using I2C or a MCP23S17 using SPI. Both ICs can be
 * utilized with the same gpioExpander class. In this example an LED is
 * attached to both GPB7 and INTA. A push button is attached to GPA7. When
 * the button is pressed it triggers an interrupt that captures the all of
 * the GPAx GPIOs, which have been configured as inputs. The state of GPAx
 * is displayed on the terminal window.
 */

#include <iostream>
#include <sstream>
#include "gpioExpander.h"

using namespace std;
using namespace exploringRPi;

int main(){
   cout << "Starting the GPIO Expander Example" << endl;
   SPIDevice *spiDevice = new SPIDevice(0,0);
   spiDevice->setSpeed(10000000);                  // MCP23S17 bus speed
   spiDevice->setMode(SPIDevice::MODE0);

// I2CDevice *i2cDevice = new I2CDevice(1, 0x20);  // for an I2C device
// GPIOExpander gpio(i2cDevice);                   // for an I2C device
   GPIOExpander gpio(spiDevice, 0x00);             // SPI dev. addr. 000
   cout << "The GPIO Expander was set up successfully" << endl;

   // PORTA are inputs and PORTB are outputs -- can mix bits
   gpio.setGPIODirections(GPIOExpander::PORTA, 0b11111111); // input=1
   gpio.setGPIODirections(GPIOExpander::PORTB, 0b00000000); // output=0
   gpio.setGPIOPullUps(GPIOExpander::PORTA, 0b10000000);    // pullup GPA7
   gpio.setInputPolarity(GPIOExpander::PORTA, 0b00000000);  // non-inverted

   // Example: get the values of PORTA and set PORTB accordingly
   unsigned char inputValues = gpio.getInputValues(GPIOExpander::PORTA);
   cout << "The values are in the form [B7,..,B0,A7,..,A0]" << endl;
   cout << "The PORTA values are: [" << gpio.getInputValuesStr() << "]\n";
   cout << "Setting PORTB to be " << (int)inputValues << endl;
   gpio.setOutputValues(GPIOExpander::PORTB, inputValues);

   // Example: attach on-change interrupt to GPIOA GPA7
   cout << "Interrupt flags[" << gpio.getInterruptFlagStateStr() << "]\n";
   cout << "Capture state[" << gpio.getInterruptCaptureStateStr() << "]\n";
   gpio.setInterruptControl(GPIOExpander::PORTA, 0b00000000);  // on change
   gpio.setInterruptOnChange(GPIOExpander::PORTA, 0b10000000); // to GPA7
   gpio.dumpRegisters();                           // display the registers
   cout << "End of the GPIO Expander Example" << endl;
}

