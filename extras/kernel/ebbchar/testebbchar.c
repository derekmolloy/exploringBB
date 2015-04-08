/**
 * @file   testebbchar.c
 * @author Derek Molloy
 * @date   7 April 2015
 * @version 0.1
 * @brief  A Linux user space program that communicates with the ebbchar.c LKM. It passes a
 * string to the LKM and reads the response from the LKM. For this example to work the device
 * must be called /dev/ebbchar.
 * @see http://www.derekmolloy.ie/ for a full description and follow-up descriptions.
*/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>

#define BUFFER_LENGTH 100                         /// The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];               /// The receive buffer from the LKM
static char *send = "Testing the EBBChar device"; /// The string to be sent to the LKM

int main(){
	int ret, fd;
	printf("Starting device test code example...\n");
	fd = open("/dev/ebbchar", O_RDWR);        /// Open the device with read/write access
	if (fd < 0){
		perror("Failed to open the device...");
		return errno;
	}
	printf("Writing message to the device [%s].\n", send);
	ret = write(fd, send, strlen(send));      /// Send the test string to the LKM
	if (ret < 0){
		perror("Failed to write the message to the device.");
		return errno;
	}
	printf("Reading from the device...\n");
	ret = read(fd, receive, BUFFER_LENGTH);   /// Read the response from the LKM
	if (ret < 0){
		perror("Failed to read the message from the device.");
		return errno;
	}
	printf("The received message is: [%s]\n", receive);
	printf("End of the program\n");
	return 0;
}
