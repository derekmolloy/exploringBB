/* A Watchdog C example
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<stdio.h>
#include<fcntl.h>
#include<linux/watchdog.h>
#define WATCHDOG "/dev/watchdog"

int main(){
   int fd, interval=30, state;
   if ((fd = open(WATCHDOG, O_RDWR))<0){
     perror("Watchdog: Failed to open watchdog device\n");
     return 1;
   }
   // set the timing interval to 30 seconds
   if (ioctl(fd, WDIOC_SETTIMEOUT, &interval)!=0){
     perror("Watchdog: Failed to set the watchdog interval\n");
     return 1;
   }
   printf("Press k to kick the dog, h to say hello and q to quit:\n");
   do{
      state = getchar();
      switch(state){
      case 'k':
         printf("[kick!]");
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
