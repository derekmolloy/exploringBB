/* A Simple Socket Example
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include "network/SocketClient.h"
using namespace std;
using namespace exploringBB;

int main(){
   cout << "Starting EBB Socket Example" << endl;
   SocketClient sc("www.google.com",80);
   sc.connectToServer();
   sc.send("GET / HTTP/1.1\r\n\r\n");
   if (sc.isClientConnected()) { cout << "Still connected " << endl; }
   string s = sc.receive(1024);
   cout << "[" << s << "]" << endl;
   cout << "End of EBB Socket Example" << endl;
}
