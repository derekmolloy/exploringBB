/* A Linux system call example
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<gnu/libc-version.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<iostream>
#include<signal.h>
using namespace std;

int main(){
   //gnu_get_libc_version() returns a string that identifies the 
   //glibc version available on the system.
   cout << "The GNU libc version is " << gnu_get_libc_version() << endl;

   // process id tid is thread identifier
   // look inside sys/syscall.h for System Call Numbers
   pid_t tid;    //pid_t is of type integer
   tid = syscall(SYS_gettid);   // make a system call to get the process id
   cout << "The Current PID is: " << tid << endl;
   //can also get by calling getpid() function from signal.h
   cout << "The Current PID is: " << getpid() << endl;

   // Can get current UserID by using: 
   int uid = syscall(SYS_getuid);
   cout << "It is being run by user with ID: " << uid << endl;
      // or getting the value from syscalls.kernelgrok.com
   uid = syscall(0xc7);
   cout << "It is being run by user with ID: " << uid << endl;

   return 0;
}
