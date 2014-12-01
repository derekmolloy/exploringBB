/** BBB Serial Slave Server
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdlib.h>

// Sends a message to the client and displays the message on the console
int message(int client, char *message){
   int size = strlen(message);
   printf(message);
   if (write(client, message, size)<0){
      perror("Failed to write to the client\n");
      return -1;
   }
   return 0;
}

// Checks to see if the command is one that is understood by the server
int processBBBCommand(int client, char *command){
   int return_val = -1;
   if (strcmp(command, "LED on")==0){
      return_val = message(client, "*** Turning the LED on  ***\n");
   }
   else if(strcmp(command, "LED off")==0){
      return_val = message(client, "*** Turning the LED off ***\n");
   }
   else if(strcmp(command, "quit")==0){
      return_val = message(client, "*** Killing the BBB Serial Server ***\n");
   }
   else {
      return_val = message(client, "*** Unknown command! ***\n");
   }
   return return_val;
}

// The main application. Must be run as root and must pass the terminal name.
int main(int argc, char *argv[]){
   int client, count=0;
   unsigned char c;
   char *command = malloc(255);

   // Is the user root?
   if(getuid()!=0){
      perror("You must run this program as root on the BBB. Exiting!\n");
      return -1;
   }

   // Was the device passed?
   if(argc!=2){
      perror("You must provide the device name: e.g., /dev/ttyO0. Exiting!\n");
      return -1;
   }

   // Set up the connection
   if ((client = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      return -1;
   }
   struct termios options;
   tcgetattr(client, &options);
   options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   tcflush(client, TCIFLUSH);
   fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);  // make the reads non-blocking
   tcsetattr(client, TCSANOW, &options);
   if (message(client, "BBB Serial Server has started running.\n")<0){
      return -1;
   }

   // Loop forever until the quit command is sent from the client or
   //  Ctrl-C is pressed on the server console
   do {
      if(read(client,&c,1)>0){
          write(STDOUT_FILENO,&c,1);
          command[count++]=c;
          if(c=='\n'){
             command[count-1]='\0';   //replace /n with /0
             processBBBCommand(client, command);
             count=0;                 //reset the command string for the next command
          }
      }
      if(read(STDIN_FILENO,&c,1)>0){  //can send text from stdin to client machine
          write(client,&c,1);
      }
   }
   while(strcmp(command,"quit")!=0);

   close(client);
   return 0;
}
