/* A callback function example
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
using namespace std;

typedef int (*CallbackType)(int);

int doSomething(CallbackType callback){
   return callback(10);  //execute callback function, pass 10
}

int callbackFunction(int var){
   cout << "I am the Callback Function! var=" << var << endl;
   return 2*var;
}

int main() {
   cout << "Hello BeagleBone" << endl;
   // pass the address of the callbackFunction() to doSomething()
   int y = doSomething(&callbackFunction);
   cout << "Value of y is: " << y << endl;
}

