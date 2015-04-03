/*
 * DCMotor.h  Created on: 12 Jun 2014
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

#ifndef DCMOTOR_H_
#define DCMOTOR_H_
#include "../gpio/GPIO.h"
#include "../gpio/PWM.h"

#define DEFAULT_DCMOTOR_PWM_PERIOD 4000
#define DEFAULT_DCMOTOR_SPEED      50.0f

namespace exploringBB {

/**
 * @class DCMotor
 * @brief A generic DC motor class that controls a motor driver board using a
 * PWM signal,and a GPIO state to control the motor direction.
 */
class DCMotor {
public:
	enum DIRECTION{ CLOCKWISE, ANTICLOCKWISE };
private:
	GPIO *gpio;
	PWM *pwm;
	float speedPercent;
	DIRECTION direction;
	void init(PWM *pwm, GPIO *gpio, DCMotor::DIRECTION direction, float speedPercent);
public:
	DCMotor(PWM *pwm, GPIO *gpio);
	DCMotor(PWM *pwm, int gpioNumber);
	DCMotor(PWM *pwm, GPIO *gpio, DCMotor::DIRECTION direction);
	DCMotor(PWM *pwm, int gpioNumber, DCMotor::DIRECTION direction);
	DCMotor(PWM *pwm, GPIO *gpio, DCMotor::DIRECTION direction, float speedPercent);
	DCMotor(PWM *pwm, int gpioNumber, DCMotor::DIRECTION direction, float speedPercent);
	virtual void go();
	virtual void setSpeedPercent(float speedPercent);
	virtual float getSpeedPercent() { return this->speedPercent; }
	virtual void setDirection(DIRECTION direction);
	virtual DIRECTION getDirection() { return this->direction; }
	virtual void reverseDirection();
	virtual void stop();
	virtual void setDutyCyclePeriod(unsigned int period_ns);
	virtual ~DCMotor();
};

} /* namespace exploringBB */

#endif /* DCMOTOR_H_ */
