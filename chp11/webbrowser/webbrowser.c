#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>

int main(int argc, char *argv[]){
   int    socketfd, portNumber, length;
   char   readBuffer[2000], message[255];
   struct sockaddr_in serverAddress; //describes endpoint to connect a socket
   struct hostent *server;           //stores information about a host name

   // The command string for a HTTP request to get / (often index.html)
   sprintf(message, "GET / HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", argv[1]);
   printf("Sending the message: %s", message);
   if (argc<=1){  // must pass the hostname
      printf("Incorrect usage, use: ./webBrowser hostname\n");
      return 2;
   }
   // gethostbyname accepts a string name and returns a host name structure
   server = gethostbyname(argv[1]);
   if (server == NULL) {
      perror("Socket Client: error - unable to resolve host name.\n");
      return 1;
   }
   // Create the socket of IP address type, SOCK_STREAM is for TCP connections
   socketfd = socket(AF_INET, SOCK_STREAM, 0);
   if (socketfd < 0){
      perror("Socket Client: error opening TCP IP-based socket.\n");
      return 1;
   }
   // clear the data in the serverAddress sockaddr_in struct
   bzero((char *) &serverAddress, sizeof(serverAddress));
   portNumber = 80;
   serverAddress.sin_family = AF_INET; //set the address family to be IP
   serverAddress.sin_port = htons(portNumber);   //set the port number to 80
   bcopy((char *)server->h_addr,(char *)&serverAddress.sin_addr.s_addr,
      server->h_length);  //set the address to the resolved hostname address

   // try to connect to the server
   if (connect(socketfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
      perror("Socket Client: error connecting to the server.\n");
      return 1;
   }
   // send the HTTP request string
   if (write(socketfd, message, sizeof(message)) < 0){
      perror("Socket Client: error writing to socket");
      return 1;
   }
   // read the HTTP response to a maximum of 2000 characters
   if (read(socketfd, readBuffer, sizeof(readBuffer)) < 0){
      perror("Socket Client: error reading from socket");
      return 1;
   }
   printf("**START**\n%s\n**END**\n", readBuffer);  //display the response
   close(socketfd);                                 //close the socket
   return 0;
}
