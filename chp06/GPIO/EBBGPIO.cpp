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
#include "GPIO.h"
#include <unistd.h>
#include <pthread.h>

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
	cout << "BeagleBone Poll Test" << endl;

	GPIO inGPIO(48);
	GPIO outGPIO(60);

	inGPIO.setDirection(INPUT);
	inGPIO.setEdgeType(RISING);

	outGPIO.setDirection(OUTPUT);

	cout << "GPIO(48) has value: " << inGPIO.getValue() << endl;
	inGPIO.setDebounceTime(200);
	inGPIO.waitForEdge(&callbackFunction);
	outGPIO.toggleOutput(100);
    cout << "Poll Started: Press the button:" << endl;
    usleep(10000000);
    cout << "Finished sleeping for 10 seconds" << endl;


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
