#include<stdio.h>
#include<stdlib.h>
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<linux/watchdog.h>
#define WATCHDOG "/dev/watchdog"

int main(){
   int fd, interval=30, state;
   if ((fd = open(WATCHDOG, O_RDWR))<0){
     perror("Watchdog: Failed to open watchdog device\n");
     return 1;
   }               // set the timing interval to 30 seconds
   if (ioctl(fd, WDIOC_SETTIMEOUT, &interval)!=0){
     perror("Watchdog: Failed to set the watchdog interval\n");
     return 1;
   }
   printf("Press f to feed the dog, h to say hello and q to quit:\n");
   do{
      state = getchar();
      switch(state){
      case 'f':
         printf("[feed!]");
         ioctl(fd, WDIOC_KEEPALIVE, NULL);
         break;
      case 'h':
         printf("[hello]");
         break;
      }
   } while (state!='q');
   printf("Closing down the application\n");
   close(fd);
   return 0;
}

