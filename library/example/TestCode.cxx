/*
 * EBBGPIO.cpp  Created on: 29 Apr 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone"
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European
 * Union Public License (the "EUPL") version 1.1 as published by the
 * European Union. Any use of this Software, other than as authorized
 * under this License is strictly prohibited (to the extent such use
 * is covered by a right of the copyright holder of this Software).
 *
 * This Software is provided under the License on an "AS IS" basis and
 * without warranties of any kind concerning the Software, including
 * without limitation merchantability, fitness for a particular purpose,
 * absence of defects or errors, accuracy, and non-infringement of
 * intellectual property rights other than copyright. This disclaimer
 * of warranty is an essential part of the License and a condition for
 * the grant of any rights to this Software.
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#include <iostream>
#include "gpio/GPIO.h"
#include "gpio/PWM.h"
#include "sensor/ADXL345.h"
#include <unistd.h>
#include <pthread.h>
#include "bus/I2CDevice.h"
#include "bus/SPIDevice.h"
#include "display/SevenSegmentDisplay.h"
#include "display/LCDCharacterDisplay.h"

using namespace std;
using namespace exploringBB;

//typedef int (*CallbackType)(int);

//int doSomething(CallbackType callback){
//   return callback(10);
//}


int callbackFunction(int var){
	cout << "BBB Button Pressed!" << var << endl;
	return var;
}

int main() {

	int32_t x = 54;


	/*if(getuid()!=0){
	      cout << "You must run this program as root. Exiting." << endl;
	      return -1;
    }*/

	/*SPIDevice spi(1,0);
	ADXL345 acc1(&spi);
	acc1.displayPitchAndRoll(100);

	I2CDevice i2c(1,0x53);
	ADXL345 acc2(&i2c);
	acc2.displayPitchAndRoll(100);*/


	LCDCharacterDisplay display(new SPIDevice(2,0), 16, 2);
	display.clear();
	display.setAutoscroll(true);

	//display.setScrollDisplayLeft(true);
	//display.setCursorPosition(0,2);
	display.print("Exploring BB");
	//usleep(2000000);

	//display.setCursorPosition(1,0);
	//display.print("by Derek Molloy");
	//usleep(2000000);

/*
	SevenSegmentDisplay disp(new SPIDevice(1,0), 2);
	//display.setNumberBase(7);
	for(int i=80; i>=0; i--){
		disp.write(i);
		usleep(250000);
	}*/



	//   SPIDevice spi(1,0);             // chip select 0 on bus 1
	//   spi.setSpeed(1000000);          // set the speed to 1 MHz
	//   cout << "The device ID is: " << (int) spi.readRegister(0x00) << endl;
	//   spi.setMode(SPIDevice::MODE3);  // set the mode to Mode3
	//   spi.writeRegister(0x2D, 0x08);  // POWER_CTL for the ADXL345
	//   spi.debugDumpRegisters(0x40);   // Dump the 64 registers from 0x00



	/*for(int i=0; i<=255; i++){
		spi.write((unsigned int)i);
		usleep(500000);
	}*/




	//cout << "The device ID is: " << (int) spi.readRegister(0x00) << endl;
	//spi.writeRegister(0x2D, 0x08); //POWER_CTL
    //spi.debugDumpRegisters(0x40);



	//spi.readRegister(0x80 + 0x32);
	//spi.readRegister(0x80 + 0x33);


	/*I2C i2c(1,0x53);
	cout << "The address is: " << (int)i2c.readRegister(0x00) << endl;
	unsigned char* data = i2c.readRegisters(0x40);
	cout << "The value of the first address is: " << (int) *data << endl;*/

	/* I2C
	ADXL345 sensor(1, 0x53);
	sensor.readSensorState();
	cout << "The x acceleration is " << sensor.getAccelerationX() << endl;
	cout << "The y acceleration is " << sensor.getAccelerationY() << endl;
	cout << "The z acceleration is " << sensor.getAccelerationZ() << endl;
	*/
	//sensor.updateSensorState();

	//sensor.debugDumpRegisters();

	/*sensor.setResolution(ADXL345::NORMAL);
	sensor.setRange(ADXL345::PLUSMINUS_4_G);
	sensor.readSensorState();
	//sensor.calculatePitchAndRoll();

	sensor.setResolution(ADXL345::HIGH);
	sensor.setRange(ADXL345::PLUSMINUS_16_G);
	sensor.readSensorState();*/

    // sensor.displayPitchAndRoll();




	//cout << "**Resolution is: " << (int)sensor.getResolution() << " and Range is: " << (int)sensor.getRange() << endl;



	/*PWM pwm("pwm_test_P9_22.15");
	//pwm.calibrateAnalogMax(3.318);
	//pwm.analogWrite(1.25);

	pwm.setPeriod(10000);
	pwm.setDutyCycle(50.0f);
	pwm.setPolarity(PWM::ACTIVE_LOW);
	pwm.run();*/

	/*pwm.setPeriod(10000);
	cout << "The period is: " << pwm.getPeriod() << endl;
    pwm.setFrequency(1000);
    cout << "The frequency is: " << pwm.getFrequency() << endl;
    cout << "The period is: " << pwm.getPeriod() << endl;
    pwm.setDutyCycle(66.66f);
    cout << "The duty cycle is: " << pwm.getDutyCycle() << endl;
    cout << "The duty cycle% is: " << pwm.getDutyCyclePercent() << endl;
    cout << "The polarity is: " << pwm.getPolarity() << endl;
    pwm.invertPolarity();
    cout << "The polarity is: " << pwm.getPolarity() << endl;
    cout << "Is running? " << pwm.isRunning() << endl;
    pwm.stop();
    cout << "Is running? " << pwm.isRunning() << endl;*/

/*	cout << "BeagleBone Poll Test" << endl;

	GPIO inGPIO(48);
	GPIO outGPIO(60);

	inGPIO.setDirection(GPIO::INPUT);
	inGPIO.setEdgeType(GPIO::RISING);
	outGPIO.setDirection(GPIO::OUTPUT);

	cout << "GPIO(48) has value: " << inGPIO.getValue() << endl;
	inGPIO.setDebounceTime(200);
	inGPIO.waitForEdge(&callbackFunction);
	outGPIO.toggleOutput(100);
    cout << "Poll Started: Press the button:" << endl;
    usleep(10000000);
    cout << "Finished sleeping for 10 seconds" << endl;*/


	//GPIO outgpio(60);
	/*gpio.setDirection(OUTPUT);
	gpio.setEdge(NONE);

	for (int i=0; i<100000; i++){
		gpio.setValue(HIGH);
		gpio.setValue(LOW);
	}*/

	/*gpio.streamOpen();
	for (int i=0; i<1000000; i++){
		gpio.streamWrite(HIGH);
		gpio.streamWrite(LOW);
	}
	gpio.streamClose();*/


	/*GPIO ingpio(48);
	ingpio.setDirection(INPUT);
	ingpio.setEdge(FALLING);
	for (int i=0; i<10; i++){
		usleep(1000000);
		cout << "GPIO(48) has value: " << ingpio.getValue() << endl;
	}*/

	//cout << "finished" << endl;
	return 0;
}
