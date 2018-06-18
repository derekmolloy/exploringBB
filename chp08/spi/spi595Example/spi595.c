/**  SPI C Seven-Segment Display Example, Written by Derek Molloy (www.derekmolloy.ie)
*    for the book Exploring BeagelBone. Based on the spidev_test.c code
*    example at www.kernel.org  */

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<stdint.h>
#include<linux/spi/spidev.h>
#define SPI_PATH "/dev/spidev1.1"

// The binary data that describes the LED state for each symbol
// A(top)         B(top right) C(bottom right)  D(bottom)
// E(bottom left) F(top left)  G(middle)        H(dot)
const unsigned char symbols[16] = {                   //(msb) HGFEDCBA (lsb)
     0b00111111, 0b00000110, 0b01011011, 0b01001111,  // 0123
     0b01100110, 0b01101101, 0b01111101, 0b00000111,  // 4567
     0b01111111, 0b01100111, 0b01110111, 0b01111100,  // 89Ab
     0b00111001, 0b01011110, 0b01111001, 0b01110001   // CdEF
};

int transfer(int fd, unsigned char send[], unsigned char rec[], int len){
   struct spi_ioc_transfer transfer;        //the transfer structure
   transfer.tx_buf = (unsigned long) send;  //the buffer for sending data
   transfer.rx_buf = (unsigned long) rec;   //the buffer for receiving data
   transfer.len = len;                      //the length of buffer
   transfer.speed_hz = 1000000;             //the speed in Hz
   transfer.bits_per_word = 8;              //bits per word
   transfer.delay_usecs = 0;                //delay in us
   transfer.cs_change = 0;       // affects chip select after transfer
   transfer.tx_nbits = 0;        // no. bits for writing (default 0)
   transfer.rx_nbits = 0;        // no. bits for reading (default 0)
   transfer.pad = 0;             // interbyte delay - check version

   // send the SPI message (all of the above fields, inc. buffers)
   int status = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
   if (status < 0) {
      perror("SPI: SPI_IOC_MESSAGE Failed");
      return -1;
   }
   return status;
}

int main(){
   int fd, i;                              // file handle and loop counter
   unsigned char null=0x00;                // sending only a single char
   uint8_t mode = 3;                       // SPI mode 3

   // The following calls set up the SPI bus properties
   if ((fd = open(SPI_PATH, O_RDWR))<0){
      perror("SPI Error: Can't open device.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_WR_MODE, &mode)==-1){
      perror("SPI: Can't set SPI mode.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_RD_MODE, &mode)==-1){
      perror("SPI: Can't get SPI mode.");
      return -1;
   }
   printf("SPI Mode is: %d\n", mode);
   printf("Counting in hexadecimal from 0 to F now:\n");
   for (i=0; i<=15; i++)
   {
      // This function can send and receive data, just sending now
      if (transfer(fd, (unsigned char*) &symbols[i], &null, 1)==-1){
         perror("Failed to update the display");
         return -1;
      }
      printf("%4d\r", i);   // print the number in the terminal window
      fflush(stdout);       // need to flush the output, no \n
      usleep(500000);       // sleep for 500ms each loop
   }
   close(fd);               // close the file
   return 0;
}
