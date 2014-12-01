/*
 * UART.h  Created on: 24 May 2014
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

#ifndef UART_H_
#define UART_H_

namespace exploringBB {

class UART {
public:
	UART();
	virtual int set_interface_attribs (int fd, int speed, int parity);
	virtual int set_blocking (int fd, int should_block);
	virtual int message(int client, char *message);
	virtual int processBBBCommand(int client, char *command);
	virtual int methodX(char *);
	virtual int go();
	virtual ~UART();
};

} /* namespace exploringBB */

#endif /* UART_H_ */
