/*
 * Servo.h  Created on: 12 May 2014
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

#ifndef SERVO_H_
#define SERVO_H_
#include<string>
#include "../gpio/PWM.h"
using std::string;

namespace exploringBB {

/**
 * @class Servo
 * @brief An extremely basic Servo class stub -- does nothing more than the PWM class but
 * is here for future use.
 */
class Servo: public PWM {
public:
	Servo(string pinName);
	virtual ~Servo();
};

} /* namespace exploringBB */

#endif /* SERVO_H_ */
