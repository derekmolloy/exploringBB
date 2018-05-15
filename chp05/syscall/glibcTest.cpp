#include<gnu/libc-version.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<pwd.h>
#include<cstdlib>
#include<sys/stat.h>
#include<iostream>
#include<signal.h>
#include<unistd.h>
using namespace std;

int main(){
   // Use helper functions to get system information:
   cout << "The GNU libc version is: " << gnu_get_libc_version() << endl;

   // Use glibc passwd struct to get user information - no error check!:
   struct passwd *pass = getpwuid(getuid());
   cout << "The current user's login is: " << pass->pw_name << endl;
   cout << "-> their full name is: " << pass->pw_gecos << endl;
   cout << "-> their user ID is: " << pass->pw_uid << endl;

   // You can use the getenv() function to get environment variables
   cout << "The user's shell is: " << getenv("SHELL") << endl;
   cout << "The user's path is: "  << getenv("PATH") << endl;

   // An example syscall to call a get the user ID -- see sys/syscall.h
   int uid = syscall(0xc7);
   cout << "Syscall gives their user ID as: " << uid << endl;

   // Call chmod directly -- type "man 2 chmod" for more information
   int ret = chmod("test.txt", 0666);
   // Can use syscall to do the same thing
   ret  = syscall(SYS_chmod, "test.txt", 0666);
   return 0;
}

