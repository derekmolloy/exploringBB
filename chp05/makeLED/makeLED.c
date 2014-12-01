/** Simple On-board LED flashing program - written in C by Derek Molloy
*    simple functional struture for the Exploring BeagleBone book
*
*    This program uses USR LED 3 and can be executed in three ways:
*         makeLED on
*         makeLED off
*         makeLED flash  (flash at 100ms intervals - on 50ms/off 50ms)
*         makeLED status (get the trigger status)
*
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LED3_PATH "/sys/class/leds/beaglebone:green:usr3"

void writeLED(char filename[], char value[]);
void removeTrigger();

int main(int argc, char* argv[]){
   if(argc!=2){
	printf("Usage is makeLEDC and one of:\n");
        printf("   on, off, flash or status\n");
	printf(" e.g. makeLED flash\n");
        return 2;
   }
   printf("Starting the makeLED program\n");
   printf("The current LED Path is: " LED3_PATH "\n");

   // select whether command is on, off, flash or status
   if(strcmp(argv[1],"on")==0){
        printf("Turning the LED on\n");
	removeTrigger();
        writeLED("/brightness", "1");
   }
   else if (strcmp(argv[1],"off")==0){
        printf("Turning the LED off\n");
	removeTrigger();
        writeLED("/brightness", "0");
   }
   else if (strcmp(argv[1],"flash")==0){
        printf("Flashing the LED\n");
        writeLED("/trigger", "timer");
        writeLED("/delay_on", "50");
        writeLED("/delay_off", "50");
   }
   else if (strcmp(argv[1],"status")==0){
      FILE* fp;   // see writeLED function below for description
      char  fullFileName[100];  
      char line[80];
      sprintf(fullFileName, LED3_PATH "/trigger"); 
      fp = fopen(fullFileName, "rt"); //reading text this time
      while (fgets(line, 80, fp) != NULL){
         printf("%s", line);
      }
      fclose(fp);  
   }
   else{
	printf("Invalid command!\n");
   }
   printf("Finished the makeLED Program\n");
   return 0;
}

void writeLED(char filename[], char value[]){
   FILE* fp;   // create a file pointer fp
   char  fullFileName[100];  // to store the path and filename
   sprintf(fullFileName, LED3_PATH "%s", filename); // write path and filename
   fp = fopen(fullFileName, "w+"); // open file for writing
   fprintf(fp, "%s", value);  // send the value to the file
   fclose(fp);  // close the file using the file pointer
}

void removeTrigger(){
  writeLED("/trigger", "none");
}

