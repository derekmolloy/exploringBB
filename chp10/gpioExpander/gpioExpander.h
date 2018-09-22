/*
 * gpioExpander.h  Created on: 17 May 2015
 * Copyright (c) 2015 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring Raspberry Pi"
 * See: www.exploringrpi.com
 * Licensed under the EUPL V.1.1 */

#ifndef GPIOEXPANDER_H_
#define GPIOEXPANDER_H_
#include "bus/SPIDevice.h"
#include "bus/I2CDevice.h"

/// The MCP23X17 has 11 register pairs (registers 0x00 to 0x1A)
#define BUFFER_SIZE 0x1A

namespace exploringRPi {

/**
 * @class GPIOExpander
 * @brief Specific class for the GPIOExpander
 */
class GPIOExpander {
private:
   I2CDevice *i2cDevice;
   SPIDevice *spiDevice;
   bool isSPIDevice;
   unsigned char spiAddress;
   unsigned char configRegister;
public:
   enum PORT { PORTA=0, PORTB=1 };
   int writeDevice(unsigned char address, unsigned char value);
   unsigned char readDevice(unsigned char address);

   GPIOExpander(I2CDevice *i2cDevice);
   GPIOExpander(SPIDevice *spiDevice, unsigned char address=0x00);

   // 16-bit -- PORTA is LSB (8-bits), PORTB is MSB (8-bits)
   virtual int setGPIODirections(PORT port, unsigned char value); // use 8-bits to set the state
   virtual int setGPIODirections(unsigned short value);

   virtual unsigned char getOutputValues(PORT port);
   virtual unsigned short getOutputValues();

   virtual std::string getOutputValuesStr();
   virtual int setOutputValues(PORT port, unsigned char value);
   virtual int setOutputValues(unsigned short value);

   virtual unsigned char getInputValues(PORT port);
   virtual unsigned short getInputValues();
   virtual std::string getInputValuesStr();

   virtual int setInputPolarity(PORT port, unsigned char value);
   virtual int setInputPolarity(unsigned short value);

   // Pull-up resistors for the input ports -- 100k Ohm value
   virtual int setGPIOPullUps(PORT port, unsigned char value);
   virtual int setGPIOPullUps(unsigned short value);

   /** Update the configuration register using 8-bits as follows:
    * bit 7: Banks = 1  -- different banks = 1, same bank = 0 (don't change from 1)
    * bit 6: Mirror = 0 -- mirror the interrupt bits = 1, don't = 0
    * bit 5: SeqOp = 1 -- addr ptr no-increment = 1, enabled = 0
    * bit 4: SlewRate = 1 -- slew rate disabled = 1, enabled = 0
    * bit 3: HardAddr = 1 -- for the MCP23S17 (enables addressing pins)
    * bit 2: OpenDrain = 0 -- open-drain o/p = 1, active driver output = 0 for INTA/B
    * bit 1: IntPolarity = 1 -- active high = 1, active low = 0
    * bit 0: Not implemented = 0  */
   virtual int updateConfigRegister(unsigned char value);

   // Sets the bits to trigger the interrupts (A and B output pins) when changed
   virtual int setInterruptOnChange(PORT port, unsigned char value);
   virtual int setInterruptOnChange(unsigned short value);

   // Get the interrupt capture register values -- reflects the value on the port
   // when the interrupt occurs. Cleared after it or GPIO state is read.
   virtual unsigned char getInterruptCaptureState(PORT port);
   virtual unsigned short getInterruptCaptureState();
   virtual std::string getInterruptCaptureStateStr();

   // Sets wheter the interrupt is configured on change or on comparison against a
   // default value (see below). 1 = on change, 0 = compare
   virtual int setInterruptControl(PORT port, unsigned char value);
   virtual int setInterruptControl(unsigned short value);

   // Sets the default comparison register -- the opposite value on an individual
   // input triggers the interrupt
   virtual int setDefaultCompareValue(PORT port, unsigned char value);
   virtual int setDefaultCompareValue(unsigned short value);

   // Get the interrupt flag register- reflects the interrupt condition on the port
   // pins of any pin that is enabled for interrupts
   virtual unsigned char getInterruptFlagState(PORT port);
   virtual unsigned short getInterruptFlagState();
   virtual std::string getInterruptFlagStateStr();

   virtual void dumpRegisters();

   virtual ~GPIOExpander() {};
};

} /* namespace exploringRPi */

#endif /* GPIOEXPANDER_H_ */
