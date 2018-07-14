#include <iostream>
#include "network/SocketServer.h"
using namespace std;
using namespace exploringBB;

int main(int argc, char *argv[]){
   cout << "Starting Beagle Board Server Example" << endl;
   SocketServer server(54321);
   cout << "Listening for a connection..." << endl;
   server.listen();
   string rec = server.receive(1024);
   cout << "Received from the client [" << rec << "]" << endl;
   string message("The Server says thanks!");
   cout << "Sending back [" << message << "]" << endl;
   server.send(message);
   cout << "End of Beagle Board Server Example" << endl;
   return 0;
}
