/* A web browser application that uses SSL applications
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <openssl/ssl.h>

int main(int argc, char *argv[]){
   int    socketfd, portNumber, length;
   char   readBuffer[2000], message[255];
   struct sockaddr_in serverAddress; //describes endpoint to connect a socket
   struct hostent *server;           //stores information about a host name

   // The command string for a HTTP request to get / (often index.html)
   sprintf(message, "GET / HTTP/1.1\r\nHost: %s\r\n\r\n", argv[1]);
   printf("Starting EBB SSL Web Browser C Example\n");
   printf("Sending the message: %s", message);
   if (argc<=1){  // must pass the hostname
      printf("Incorrect usage, use: ./webBrowserSSL hostname\n");
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
   portNumber = 443;
   serverAddress.sin_family = AF_INET; //set the address family to be IP
   serverAddress.sin_port = htons(portNumber);   //set the port number to 80
   bcopy((char *)server->h_addr,(char *)&serverAddress.sin_addr.s_addr,
      server->h_length);  //set the address to the resolved hostname address

   // try to connect to the server
   if (connect(socketfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
      perror("Socket Client: error connecting to the server.\n");
      return 1;
   }

   // Register the error strings for libssl functions
   SSL_load_error_strings();
   // Register the SSL/TLS ciphers and digests
   SSL_library_init();
   // Create a new SSL context object to establish TLS/SSL enabled connections
   SSL_CTX *ssl_ctx = SSL_CTX_new(SSLv23_client_method());
   // Attaching SSL Connection to the socket
   SSL *conn = SSL_new(ssl_ctx); // create a SSL structure for a SSL session
   SSL_set_fd(conn, socketfd);   // Assign a socket to a SSL structure
   SSL_connect(conn);            // Start a SSL session with a remote server

   // send data across a SSL session
   if (SSL_write(conn, message, sizeof(message)) < 0){
      perror("Socket Client: error writing to the SSl socket");
      return 1;
   }
   // read data scross a SSL session
   if (SSL_read(conn, readBuffer, sizeof(readBuffer)) < 0){
      perror("Socket Client: error reading from the SSL socket");
      return 1;
   }
   printf("**START**\n%s\n**END**\n", readBuffer);  //display the response
   SSL_free(conn);                                  //free the connection
   close(socketfd);                                 //close the socket
   SSL_CTX_free(ssl_ctx);                           //free the SSL context
   printf("End of EBB SSL Socket Example\n");
   return 0;
}
