/*
 * gpioExpander.cpp  Created on: 17 May 2015
 * Copyright (c) 2015 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring Raspberry Pi"
 * See: www.exploringrpi.com
 * Licensed under the EUPL V.1.1  */

#include "gpioExpander.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <sstream>
#include <bitset>
#include <iomanip>
using namespace std;

namespace exploringRPi {

//From Table 1-3 for the MCP23X17 Data sheet
#define IODIRA   0x00   //< Input/Output direction Port A Register
#define IODIRB   0x01   //< Input/Output direction Port B Register
#define IPOLA    0x02   //< Set the polarity of Port A (invert inputs)
#define IPOLB    0x03   //< Set the polarity of Port B (invert inputs)
#define GPINTENA 0x04   //< Interrupt on change control reg. Port A
#define GPINTENB 0x05   //< Port B -- must have DEFVAL and INTCON set
#define DEFVALA  0x06   //< Default compare register
#define DEFVALB  0x07   //< Default compare register
#define INTCONA  0x08   //< Interrupt control register to choose whether int
#define INTCONB  0x09   //< is on change (1) or on compare with DEFVAL (0)
#define IOCON    0x0A   //< IOCON is shared with IOCONA and IOCONB
#define IOCONA   0x0A   //< Configuration and control register
#define IOCONB   0x0B   //< Configuration and control register
#define GPPUA    0x0C   //< Input pull-up resistor config for Port A
#define GPPUB    0x0D   //< Input pull-up resistor config for Port B
#define INTFA    0x0E   //< Interrupt flag register (reflects int cond.)
#define INTFB    0x0F   //< Interrupt flag register Port B
#define INTCAPA  0x10   //< Captures Port A values when interrupt occurs
#define INTCAPB  0x11   //< Captures Port B values when interrupt occurs
#define GPIOA    0x12   //< GPIO input register (writing affects OLATx)
#define GPIOB    0x13   //< GPIO input register Port B
#define OLATA    0x14   //< Output latch for setting outputs on Port A
#define OLATB    0x15   //< Output latch for setting outputs on Port B

#define DEFAULT_INIT_STATE  0b00111010  // Be careful not to change!

int GPIOExpander::writeDevice(unsigned char address, unsigned char value){
   if(isSPIDevice){
      this->spiDevice->open();
      unsigned char send[3], null_return = 0x00;
      send[0]=0b01000000;
      send[0] = send[0] | (this->spiAddress << 1);
      send[1]=address;
      send[2]=value;
      this->spiDevice->transfer(send, &null_return, 3); // R/W bit is cleared
      this->spiDevice->close();
      return 0;
   }
   return this->i2cDevice->writeRegister(address, value);
}

unsigned char GPIOExpander::readDevice(unsigned char address){
   if(isSPIDevice){
      this->spiDevice->open();
      unsigned char send[3],receive[3];
      send[0]=0b01000001;
      send[0] = send[0] | (this->spiAddress << 1);
      send[1]=address;
      send[2]=0x00;
      this->spiDevice->transfer(send, receive, 3); // R/W bit is cleared
      this->spiDevice->close();
      return receive[2];
   }
   return this->i2cDevice->readRegister(address);
}

/**
 * The constructor for the GPIO Expander object. It passes the bus number and the
 * device address (which is 0x40 by default) to the constructor of I2CDevice. All of the states
 * are initialized and the registers are updated.
 * @param I2CBus The bus number that the MCP23X17 is on (default 1)
 * @param I2CAddress The address of the MCP23X17 device (default 0x40)
 */

GPIOExpander::GPIOExpander(I2CDevice *i2cDevice) {
   this->i2cDevice = i2cDevice;
   this->isSPIDevice = false;
   this->updateConfigRegister(DEFAULT_INIT_STATE);
}

GPIOExpander::GPIOExpander(SPIDevice *spiDevice, unsigned char spiAddress) {
   if((spiAddress < 0) | (spiAddress>0x07)){
      this->spiAddress = 0x00;
   }
   else {
      this->spiAddress = spiAddress;
   }
   this->spiDevice = spiDevice;
   this->isSPIDevice = true;
   this->updateConfigRegister(DEFAULT_INIT_STATE);
}

int GPIOExpander::updateConfigRegister(unsigned char value){
   this->configRegister = value;
   //IOCON is mirrored over IOCONA and IOCONB
   this->writeDevice(IOCONA, configRegister);
   return this->writeDevice(IOCONB, configRegister);
}

int GPIOExpander::setGPIODirections(GPIOExpander::PORT port, unsigned char value) {
   if(port==PORTA){
      return this->writeDevice(IODIRA, value);
   }
   return this->writeDevice(IODIRB, value);
}

int GPIOExpander::setGPIODirections(unsigned short value){
   this->setGPIODirections(PORTA, value & 0xff);
   return this->setGPIODirections(PORTB, value >> 8);
}

unsigned char GPIOExpander::getOutputValues(GPIOExpander::PORT port) {
   if(port==PORTA){
      return this->readDevice(OLATA);
   }
   return this->readDevice(OLATB);
}

unsigned short GPIOExpander::getOutputValues(){
   short value = getOutputValues(PORTB);
   return (value<<8) | getOutputValues(PORTA);
}

std::string GPIOExpander::getOutputValuesStr(){
   stringstream ss;
   ss << std::bitset<16>(getOutputValues());
   return ss.str();
}

int GPIOExpander::setOutputValues(GPIOExpander::PORT port, unsigned char value) {
   if(port==PORTA){
      return this->writeDevice(OLATA, value);
   }
   return this->writeDevice(OLATB, value);
}

int GPIOExpander::setOutputValues(unsigned short value){
   setOutputValues(PORTA, value & 0xff);
   return setOutputValues(PORTB, value >> 8);
}

unsigned char GPIOExpander::getInputValues(GPIOExpander::PORT port) {
   if(port==PORTA){
      return this->readDevice(GPIOA);
   }
   return this->readDevice(GPIOB);
}

unsigned short GPIOExpander::getInputValues(){
   short value = getInputValues(PORTB);
   return (value<<8) | getInputValues(PORTA);
}

std::string GPIOExpander::getInputValuesStr(){
   stringstream ss;
   ss << std::bitset<16>(getInputValues());
   return ss.str();
}

int GPIOExpander::setInputPolarity(GPIOExpander::PORT port, unsigned char value){
   if(port==PORTA){
      return this->writeDevice(IPOLA, value);
   }
   return this->writeDevice(IPOLB, value);
}

int GPIOExpander::setInputPolarity(unsigned short value){
   setInputPolarity(PORTA, value & 0xff);
   return setInputPolarity(PORTB, value >> 8);
}

int GPIOExpander::setGPIOPullUps(GPIOExpander::PORT port, unsigned char value){
   if(port==PORTA){
      return this->writeDevice(GPPUA, value);
   }
   return this->writeDevice(GPPUB, value);
}

int GPIOExpander::setGPIOPullUps(unsigned short value){
   setGPIOPullUps(PORTA, value & 0xff);
   return setGPIOPullUps(PORTB, value >> 8);
}

// Sets the bits to trigger the interrupts (A and B output pins) when changed
int GPIOExpander::setInterruptOnChange(GPIOExpander::PORT port, unsigned char value){
   if(port==PORTA){
      return this->writeDevice(GPINTENA, value);
   }
   return this->writeDevice(GPINTENB, value);
}

int GPIOExpander::setInterruptOnChange(unsigned short value){
   setInterruptOnChange(PORTA, value & 0xff);
   return setInterruptOnChange(PORTB, value >> 8);
}

// Get the interrupt capture register values -- reflects the value on the port
// when the interrupt occurs. Cleared after it or GPIO state is read.
unsigned char GPIOExpander::getInterruptCaptureState(GPIOExpander::PORT port){
   if(port==PORTA){
      return this->readDevice(INTCAPA);
   }
   return this->readDevice(INTCAPB);
}

unsigned short GPIOExpander::getInterruptCaptureState(){
   short value = getInterruptCaptureState(PORTB);
   return (value<<8) | getInterruptCaptureState(PORTA);
}

std::string GPIOExpander::getInterruptCaptureStateStr(){
   stringstream ss;
   ss << std::bitset<16>(getInterruptCaptureState());
   return ss.str();
}

int GPIOExpander::setInterruptControl(GPIOExpander::PORT port, unsigned char value){
   if(port==PORTA){
      return this->writeDevice(INTCONA, value);
   }
   return this->writeDevice(INTCONB, value);
}
int GPIOExpander::setInterruptControl(unsigned short value){
   setInterruptControl(PORTA, value & 0xff);
   setInterruptControl(PORTB, value >> 8);
}

int GPIOExpander::setDefaultCompareValue(GPIOExpander::PORT port, unsigned char value){
   if(port==PORTA){
      return this->writeDevice(DEFVALA, value);
   }
   return this->writeDevice(DEFVALB, value);
}
int GPIOExpander::setDefaultCompareValue(unsigned short value){
   setDefaultCompareValue(PORTA, value & 0xff);
   return setDefaultCompareValue(PORTB, value >> 8);
}

unsigned char GPIOExpander::getInterruptFlagState(GPIOExpander::PORT port){
   if(port==PORTA){
      return this->readDevice(INTFA);
   }
   return this->readDevice(INTFB);
}

unsigned short GPIOExpander::getInterruptFlagState(){
   short value = getInterruptFlagState(PORTB);
   return (value<<8) | getInterruptFlagState(PORTA);
}

std::string GPIOExpander::getInterruptFlagStateStr(){
   stringstream ss;
   ss << std::bitset<16>(getInterruptFlagState());
   return ss.str();
}

void GPIOExpander::dumpRegisters(){
  const string registers[] = {"IODIRA  ","IPOLA   ","GPINTENA","DEFVALA ","INTCONA ",
      "IOCONA  ", "GPPUA   ","INTFA   ","INTAPA  ","GPIOA   ","OLATA   "};
  cout << "Register Dump:" << endl;
  for(int i=0; i<11; i++){
     char a = this->readDevice(i*2);
     char b = this->readDevice((i*2)+1);
     cout << "Register " << registers[i] << ": " << setw(5) << (int)a << "  B: " << (int)b << endl;
  }
}

} /* namespace exploringRPi */
