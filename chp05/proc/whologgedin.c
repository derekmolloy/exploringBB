/* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<unistd.h>
#include<stdio.h>
int main(){
   printf("The user logged in is %s\n", getlogin());
   return 0;
}
