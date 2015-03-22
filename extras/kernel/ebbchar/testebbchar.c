#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>

#define BUFFER_LENGTH 100
static char receive[BUFFER_LENGTH];
static char *send = "Testing the EBBChar device";

int main(){
	int ret, fd;

	printf("Starting device test code example...\n");
	fd = open("/dev/ebbchar", O_RDWR);
	if (fd < 0){
		perror("Failed to open the device...");
		return errno;
	}
	printf("Writing message to the device [%s].\n", send);
	ret = write(fd, send, strlen(send));
	if (ret < 0){
		perror("Failed to write the message to the device.");
		return errno;
	}
	printf("Reading from the device...\n");
	ret = read(fd, receive, BUFFER_LENGTH);
	if (ret < 0){
		perror("Failed to read the message from the device.");
		return errno;
	}
	printf("The received message is: [%s]\n", receive);
	printf("End of the program\n");
	return 0;
}
