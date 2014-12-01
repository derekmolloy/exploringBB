/*
 * DCMotor.cpp  Created on: 12 Jun 2014
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

#include "DCMotor.h"

namespace exploringBB {

DCMotor::DCMotor(PWM *pwm, GPIO *gpio) {
	init(pwm, gpio, CLOCKWISE, DEFAULT_DCMOTOR_SPEED);
}

DCMotor::DCMotor(PWM *pwm, int gpioNumber) {
	this->gpio = new GPIO(gpioNumber);
	this->gpio->setDirection(GPIO::OUTPUT);
	init(pwm, this->gpio, CLOCKWISE, DEFAULT_DCMOTOR_SPEED);
}

DCMotor::DCMotor(PWM *pwm, GPIO *gpio, DCMotor::DIRECTION direction) {
	init(pwm, gpio, direction, DEFAULT_DCMOTOR_SPEED);
}

DCMotor::DCMotor(PWM *pwm, int gpioNumber, DCMotor::DIRECTION direction) {
	this->gpio = new GPIO(gpioNumber);
	this->gpio->setDirection(GPIO::OUTPUT);
	init(pwm, this->gpio, direction, DEFAULT_DCMOTOR_SPEED);
}

DCMotor::DCMotor(PWM *pwm, GPIO *gpio, DCMotor::DIRECTION direction, float speedPercent) {
	init(pwm, gpio, direction, speedPercent);
}

DCMotor::DCMotor(PWM *pwm, int gpioNumber, DCMotor::DIRECTION direction, float speedPercent) {
	this->gpio = new GPIO(gpioNumber);
	this->gpio->setDirection(GPIO::OUTPUT);
	init(pwm, this->gpio, direction, speedPercent);
}

void DCMotor::init(PWM *pwm, GPIO *gpio, DCMotor::DIRECTION direction, float speedPercent){
	this->pwm = pwm;
	this->gpio = gpio;
	this->gpio->setDirection(GPIO::OUTPUT);
	this->direction = direction;
	this->speedPercent = speedPercent;
	this->stop();
	this->pwm->setPolarity(PWM::ACTIVE_LOW);
	this->setDirection(this->direction);
	this->setDutyCyclePeriod(DEFAULT_DCMOTOR_PWM_PERIOD);
	this->setSpeedPercent(this->speedPercent);
}

void DCMotor::setSpeedPercent(float speedPercentage){
	this->pwm->setDutyCycle(speedPercentage);
	this->speedPercent = speedPercentage;
}

void DCMotor::setDirection(DIRECTION direction){
	if(direction == CLOCKWISE){
		this->gpio->setValue(GPIO::HIGH);
	}
	else{
		this->gpio->setValue(GPIO::LOW);
	}
	this->direction = direction;
}

void DCMotor::reverseDirection(){
	if(this->direction == CLOCKWISE){
		this->setDirection(ANTICLOCKWISE);
	}
	else{
		this->setDirection(CLOCKWISE);
	}
}

void DCMotor::stop(){
	this->pwm->stop();
}

void DCMotor::go(){
	this->pwm->run();
}

void DCMotor::setDutyCyclePeriod(unsigned int period_ns){
	this->pwm->setPeriod(period_ns);
}

DCMotor::~DCMotor() {
	delete gpio;
}

} /* namespace exploringBB */
