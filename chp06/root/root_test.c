#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>

int main(){
   printf("Start of GPIO test program.\n");
   if(geteuid()!=0) {    // Get effective UID
      printf("You must run this program as root. Exiting.\n");
      return -EPERM;
   }
   printf("Doing something here with root privileges!\n");
   return 0;
}
