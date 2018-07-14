#include <iostream>
#include "network/SocketClient.h"
using namespace std;
using namespace exploringBB;

int main(int argc, char *argv[]){
   if(argc!=2){
      cout << "Incorrect usage: " << endl;
      cout << "   client server_name" << endl;
      return 2;
   }
   cout << "Starting Beagle Board Client Example" << endl;
   SocketClient sc(argv[1], 54321);
   sc.connectToServer();
   string message("Hello from the Client");
   cout << "Sending [" << message << "]" << endl;
   sc.send(message);
   string rec = sc.receive(1024);
   cout << "Received [" << rec << "]" << endl;
   cout << "End of Beagle Board Client Example" << endl;
}
