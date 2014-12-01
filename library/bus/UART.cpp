/*
 * UART.cpp  Created on: 24 May 2014
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

#include "UART.h"
#include<iostream>
#include<string>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

namespace exploringBB {

UART::UART() {
	// TODO Auto-generated constructor stub

}

int UART::message(int client, char *message){
   int size = strlen(message);
   printf(message);
   if (write(client, message, size)<0){
      perror("Failed to write to the client\n");
      return -1;
   }
   return 0;
}

// Checks to see if the command is one that is understood by the server
int UART::processBBBCommand(int client, char *command){
   int return_val = -1;
   if (strcmp(command, "LED on")==0){
      return_val = message(client, "*** Turning the LED on  ***\n");
   }
   else if(strcmp(command, "LED off")==0){
      return_val = message(client, "*** Turning the LED off ***\n");
   }
   else if(strcmp(command, "quit")==0){
      return_val = message(client, "*** Killing the BBB Serial Server ***\n");
   }
   else {
      return_val = message(client, "*** Unknown command! ***\n");
   }
   return return_val;
}

int UART::methodX(char *argv){
	int client, count=0;
	   unsigned char c;
	   char command[255];

	   // Is the user root?
	   if(getuid()!=0){
	      perror("You must run this program as root on the BBB. Exiting!\n");
	      return -1;
	   }

	   // Set up the connection
	   if ((client = open(argv, O_RDWR | O_NOCTTY | O_NDELAY))<0){
	      perror("UART: Failed to open the file.\n");
	      return -1;
	   }
	   struct termios options;
	   tcgetattr(client, &options);
	   options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
	   options.c_iflag = IGNPAR | ICRNL;
	   tcflush(client, TCIFLUSH);
	   fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);  // make the reads non-blocking
	   tcsetattr(client, TCSANOW, &options);
	   if (message(client, "BBB Serial Server has started running.\n")<0){
	      return -1;
	   }

	   // Loop forever until the quit command is sent from the client or
	   //  Ctrl-C is pressed on the server console
	   do {
	      if(read(client,&c,1)>0){
	          write(STDOUT_FILENO,&c,1);
	          command[count++]=c;
	          if(c=='\n'){
	             command[count-1]='\0';   //replace /n with /0
	             processBBBCommand(client, command);
	             count=0;                 //reset the command string for the next command
	          }
	      }
	      if(read(STDIN_FILENO,&c,1)>0){  //can send text from stdin to client machine
	          write(client,&c,1);
	      }
	   }
	   while(strcmp(command,"quit")!=0);

	   close(client);
	   return 0;
}



int UART::set_interface_attribs (int fd, int speed, int parity)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
			perror("error %d from tcgetattr");
			return -1;
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
	// disable IGNBRK for mismatched speed tests; otherwise receive break
	// as \000 chars
	tty.c_iflag &= ~IGNBRK;         // disable break processing
	tty.c_lflag = 0;                // no signaling chars, no echo,
									// no canonical processing
	tty.c_oflag = 0;                // no remapping, no delays
	tty.c_cc[VMIN]  = 0;            // read doesn't block
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

	tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
									// enable reading
	tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
	{
			perror("Error from tcsetattr");
			return -1;
	}
	return 0;
}

int UART::set_blocking (int fd, int should_block)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
			perror("error from tggetattr");
			return -1;
	}

	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
			perror("error setting term attributes");
	return 0;
}

int UART::go(){
	string portname = "/dev/ttyO4";
	int fd = open(portname.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{
			perror("UART: Error on opening port");
			return -1;
	}

	set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
	set_blocking (fd, 0);                // set no blocking

	write (fd, "hello!\n", 7);           // send 7 character greeting

	usleep ((7 + 25) * 100);             // sleep enough to transmit the 7 plus
										 // receive 25:  approx 100 uS per char transmit
	char buf [100];
	return read (fd, buf, sizeof buf);  // read up to 100 characters if ready to read
}


UART::~UART() {
	// TODO Auto-generated destructor stub
}

} /* namespace exploringBB */
