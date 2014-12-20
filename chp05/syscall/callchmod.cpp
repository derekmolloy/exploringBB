/* A Linux chmod system call example
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root
* directory for copyright and GNU GPLv3 license information.            */

#include<gnu/libc-version.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<iostream>
#include<signal.h>
using namespace std;

int main(){
   //gnu_get_libc_version() returns a string that identifies the 
   //glibc version available on the system.
   cout << "The GNU libc version is " << gnu_get_libc_version() << endl;

   cout << "Calling chmod using a system call:" << endl;
   int ret  = syscall(SYS_chmod, "test.txt", 0777);
   cout << "The return value is " << ret << endl;
   cout << "Reset the permissions using: chmod 644 test.txt" << endl;

   return 0;
}
