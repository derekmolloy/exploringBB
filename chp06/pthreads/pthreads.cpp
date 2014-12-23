/*
 * PThreadsExample.cpp  Created on: 29 Apr 2014
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
#include <pthread.h>
#include <unistd.h>
using namespace std;

// This is the thread function that will execute when the thread is created
//  it passes and receives data by void pointers
void *threadFunction(void *value){
	int *x = (int *)value;    //cast the data passed to an int pointer
	while(*x<5){              //while the value of x is less than 5
		usleep(1000);         //sleep for 1ms - encourage main thread
		(*x)++;               //increment the value of x by 1
	}
	return x;                 //return the pointer x (as a void*)
}

int main() {
    int x=0, y=0;
    pthread_t thread;         //this is our handle to the pthread
    // create the thread, pass the reference, address of the function and data
    // pthread_create() returns 0 on the successful creation of a thread
    if(pthread_create(&thread, NULL, &threadFunction, &x)!=0){
    	cout << "Failed to create the thread" << endl;
    	return 1;
    }
    // at this point the thread was created successfully
    while(y<5){               // loop and increment y, displaying values
    	cout << "The value of x=" << x << " and y=" << y++ << endl;
    	usleep(1000);         // encourage the pthread to run
    }
    void* result;             // OPTIONAL: receive data back from pthread
    pthread_join(thread, &result);   // allow the pthread to complete
    int *z = (int *) result;         // cast from void* to int* to get z
    cout << "Final: x=" << x << ", y=" << y << " and z=" << *z << endl;
    return 0;
}
