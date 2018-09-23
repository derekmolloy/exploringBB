#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#define BUFFER_SIZE 5      //0x00 to 0x04

int main(){
   int file;
   printf("Starting the Arduino I2C temperature application\n");
   if((file=open("/dev/i2c-1", O_RDWR)) < 0){
      perror("failed to open the bus\n");
      return 1;
   }
   if(ioctl(file, I2C_SLAVE, 0x44) < 0){
      perror("Failed to connect to the Arduino\n");
      return 1;
   }
   char writeBuffer[5] = {0x00, 0x01, 0x02, 0x03, 0x04};
   if(write(file, writeBuffer, 5)!=5){
      perror("Failed to reset the read address\n");
      return 1;
   }
   char buf[BUFFER_SIZE];
   if(read(file, buf, BUFFER_SIZE)!=BUFFER_SIZE){
      perror("Failed to read in the registers\n");
      return 1;
   }
   printf("The temperature is %d.%d°C\n", buf[0], buf[1]);
   printf(" which is %d.%d°F\n", buf[2], buf[3]);
   printf("The alert temperature is %d°C\n", buf[4]);
   close(file);
   return 0;
}

