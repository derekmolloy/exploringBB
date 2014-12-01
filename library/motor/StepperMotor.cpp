/*
 * StepperMotor.cpp  Created on: 13 Jun 2014
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

#include "StepperMotor.h"
#include<iostream>
#include<unistd.h>
#include<cmath>
using namespace std;

namespace exploringBB {

StepperMotor::StepperMotor(GPIO *gpio_MS1, GPIO *gpio_MS2, GPIO *gpio_STEP, GPIO *gpio_SLP,
		 GPIO *gpio_DIR, int speedRPM, int stepsPerRevolution){
	this->gpio_MS1  = gpio_MS1;
	this->gpio_MS2  = gpio_MS2;
	this->gpio_STEP = gpio_STEP;
	this->gpio_SLP  = gpio_SLP;
	this->gpio_DIR  = gpio_DIR;
	// the default speed in rpm
	this->setSpeed(speedRPM);
	this->stepsPerRevolution = stepsPerRevolution;
	this->init(speedRPM, stepsPerRevolution);
}

StepperMotor::StepperMotor(int gpio_MS1, int gpio_MS2, int gpio_STEP, int gpio_SLP,
 			int gpio_DIR, int speedRPM, int stepsPerRevolution){
	this->gpio_MS1 = new GPIO(gpio_MS1);
	this->gpio_MS2 = new GPIO(gpio_MS2);
	this->gpio_STEP = new GPIO(gpio_STEP);
	this->gpio_SLP = new GPIO(gpio_SLP);
	this->gpio_DIR = new GPIO(gpio_DIR);
	this->gpio_MS1->setDirection(GPIO::OUTPUT);
	this->gpio_MS2->setDirection(GPIO::OUTPUT);
	this->gpio_STEP->setDirection(GPIO::OUTPUT);
	this->gpio_SLP->setDirection(GPIO::OUTPUT);
	this->gpio_DIR->setDirection(GPIO::OUTPUT);
	this->init(speedRPM, stepsPerRevolution);
}

void StepperMotor::init(int speedRPM, int stepsPerRevolution){
	this->gpio_MS1->setDirection(GPIO::OUTPUT);
	this->gpio_MS2->setDirection(GPIO::OUTPUT);
	this->gpio_STEP->setDirection(GPIO::OUTPUT);
	this->gpio_SLP->setDirection(GPIO::OUTPUT);
	this->gpio_DIR->setDirection(GPIO::OUTPUT);
	this->threadRunning = false;

	this->stepsPerRevolution = stepsPerRevolution;
	// default to clockwise direction
	this->setDirection(StepperMotor::CLOCKWISE);
	// default to full stepping
	setStepMode(STEP_FULL);
	// the default number of steps per revolution
	setStepsPerRevolution(stepsPerRevolution);
	// the default speed in rpm
	this->setSpeed(speedRPM);
	//wake up the controller - holding torque..
	this->wake();
}

void StepperMotor::setStepMode(STEP_MODE mode) {
	this->stepMode = mode;
	switch(stepMode){
	case STEP_FULL:
		this->gpio_MS1->setValue(GPIO::LOW);
		this->gpio_MS2->setValue(GPIO::LOW);
		this->delayFactor = 1;
		break;
	case STEP_HALF:
		this->gpio_MS1->setValue(GPIO::HIGH);
		this->gpio_MS2->setValue(GPIO::LOW);
		this->delayFactor = 2;
		break;
	case STEP_QUARTER:
		this->gpio_MS1->setValue(GPIO::LOW);
		this->gpio_MS2->setValue(GPIO::HIGH);
		this->delayFactor = 4;
		break;
	case STEP_EIGHT:
		this->gpio_MS1->setValue(GPIO::HIGH);
		this->gpio_MS2->setValue(GPIO::HIGH);
		this->delayFactor = 8;
		break;
	}
}

void StepperMotor::setSpeed(float rpm) {
	this->speed = rpm;
	float delayPerSec = (60/rpm)/stepsPerRevolution;    // delay per step in seconds
	this->uSecDelay = (int)(delayPerSec * 1000 * 1000); // in microseconds
}

void StepperMotor::step(int numberOfSteps){
	//cout << "Doing "<< numberOfSteps << " steps and going to sleep for " << uSecDelay/delayFactor << "uS\n";
	int sleepDelay = uSecDelay/delayFactor;
	if(numberOfSteps<0) {
		this->reverseDirection();
		numberOfSteps = -numberOfSteps;
	}
	for(int i=0; i<numberOfSteps; i++){
		this->step();
		usleep(sleepDelay);
	}
}

void StepperMotor::step(){
    this->gpio_STEP->setValue(GPIO::LOW);
    this->gpio_STEP->setValue(GPIO::HIGH);
}


int  StepperMotor::threadedStepForDuration(int numberOfSteps, int duration_ms){
	this->threadedStepNumber = numberOfSteps;
	this->threadedStepPeriod = duration_ms/numberOfSteps;
	this->threadRunning = true;
    if(pthread_create(&this->thread, NULL, &threadedStep, static_cast<void*>(this))){
    	perror("StepperMotor: Failed to create the stepping thread");
    	this->threadRunning = false;
    	return -1;
    }
    return 0;
}

void StepperMotor::setDirection(DIRECTION direction){
	this->direction = direction;
	if(this->direction==CLOCKWISE) this->gpio_DIR->setValue(GPIO::HIGH);
		else this->gpio_DIR->setValue(GPIO::LOW);
}

void StepperMotor::reverseDirection(){
	if(this->direction==CLOCKWISE){
		this->setDirection(ANTICLOCKWISE);
	}
	else this->setDirection(CLOCKWISE);
}

void StepperMotor::rotate(float degrees){
	float degreesPerStep = 360.0f/getStepsPerRevolution();
	int numberOfSteps = floor(((this->delayFactor*degrees)/degreesPerStep)+0.5);
	//cout << "The number of steps is " << numberOfSteps << endl;
	//cout << "The delay factor is " << delayFactor << endl;
	step(numberOfSteps);
}

void StepperMotor::sleep(){
	this->asleep = true;
	this->gpio_SLP->setValue(GPIO::LOW);
}

void StepperMotor::wake(){
	this->asleep = false;
	this->gpio_SLP->setValue(GPIO::HIGH);
}

StepperMotor::~StepperMotor() {}

// This thread function is a friend function of the class
void* threadedStep(void *value){
	StepperMotor *stepper = static_cast<StepperMotor*>(value);
	while(stepper->threadRunning){
		stepper->step();
		usleep(stepper->threadedStepPeriod * 1000);  // convert from ms to us
		if(stepper->threadedStepNumber>0) stepper->threadedStepNumber--;
		if(stepper->threadedStepNumber==0) stepper->threadRunning = false;
	}
	return 0;
}

} /* namespace exploringBB */
