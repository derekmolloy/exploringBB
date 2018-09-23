#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#define BUFFER_SIZE 5      //0x00 to 0x04

int main(int argc, char **argv){
   int file, i, alert=0xFF;
   printf("Starting the Arduino I2C temperature application\n");

   // check if alert temperature argument passed
   if(argc==2){           // convert argument string to int value
      if (sscanf(argv[1],"%i",&alert)!=1) {
         perror("Failed to read the alert temperature\n");
         return 1;
      }
      if (alert>255 || alert<0) {
         perror("Alert temperature is outside of range\n");
         return 1;
      }
   }
   if((file=open("/dev/i2c-1", O_RDWR)) < 0){
      perror("failed to open the bus\n");
      return 1;
   }
   if(ioctl(file, I2C_SLAVE, 0x44) < 0){
      perror("Failed to connect to the Arduino\n");
      return 1;
   }
   char rec[BUFFER_SIZE], send;
   for(i=0; i<BUFFER_SIZE; i++){       // sending char by char
     send = (char) i;
     if(write(file, &send, 1)!=1){
        perror("Failed to request a register\n");
        return 1;
     }
     if(read(file, &rec[i], 1)!=1){
        perror("Failed to read in the data\n");
        return 1;
     }
   }
   printf("The temperature is %d.%d°C\n", rec[0], rec[1]);
   printf(" which is %d.%d°F\n", rec[2], rec[3]);
   printf("The alert temperature is %d°C\n", rec[4]);

   if(alert!=0xFF) {
     char alertbuf[] = {0x04, 0};     // writing alert to 0x04
     alertbuf[1] = (char) alert;      // value read as argument
     printf("Setting alert temperature to %d°C\n", alert);
     if(write(file, alertbuf, 2)!=2){
        perror("Failed to set the alert temperature!\n");
        return 1;
     }
   }
   close(file);
   return 0;
}
