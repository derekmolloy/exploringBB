/* A program for a threaded Client/Server
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include "network/SocketServer.h"
using namespace std;
using namespace exploringBB;

int main(int argc, char *argv[]){
   cout << "Starting EBB Server Example" << endl;
   SocketServer server(54321);
   cout << "Listening for a connection..." << endl;
   server.threadedListen();
//   string rec = server.receive(1024);
//   cout << "Received from the client [" << rec << "]" << endl;
//   string message("The Server says thanks!");
//   cout << "Sending back [" << message << "]" << endl;
//   server.send(message);
   cout << "End of EBB Server Example" << endl;
}
