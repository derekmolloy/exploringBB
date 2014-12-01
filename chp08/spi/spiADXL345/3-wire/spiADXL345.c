/* Sample SPI Example Program to read the Device ID of the ADXL345 on the
*  SPI bus. Written by Derek Molloy (derekmolloy.ie) for the book Exploring
*  Beaglebone.
*
*  To read to the ADXL345 the leading bit of the address must be
*  high. The Device ID of the ADXL345 is 0xE5, which is 229 in decimal, so
*  this should be the value returned. This code is based on the code at:
*  https://www.kernel.org/doc/Documentation/spi/  
*
*  Modified for 3-wire communication
*/

#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdint.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/spi/spidev.h>

// You need to correctly identify the SPI device bus and number
static const char *device = "/dev/spidev1.0";

int main(){
   unsigned char send[4], receive[4];
   send[0]=  0x80;        //going to write to the dataformat register
   send[1]=  0x40;        //turning 3-wire on on the sensor 0x01000000
   send[2]=  0x80;
   //send[3]= (0x80+0x00);  //want to read address 0x00 on the ADXL345
   uint8_t mode=3, bits=8;
   uint32_t speed=1000000;
   int file;
   struct spi_ioc_transfer transfer = {
      .tx_buf = (unsigned long) send,
      .rx_buf = (unsigned long) receive,
      .len = 2,
      .delay_usecs = 1,
      .cs_change = 1
   };

   if ((file = open(device, O_RDWR))<0){
      perror("SPI: Can't open device.");
      return -1;
   }
   if (ioctl(file, SPI_IOC_WR_MODE, &mode)==-1){
      perror("SPI: Can't set SPI mode.");
      return -1;
   }
   if (ioctl(file, SPI_IOC_RD_MODE, &mode)==-1){
      perror("SPI: Can't get SPI mode.");
      return -1;
   }
   if (ioctl(file, SPI_IOC_WR_BITS_PER_WORD, &bits)==-1){
      perror("SPI: Can't set bits per word.");
      return -1;
   }
   if (ioctl(file, SPI_IOC_RD_BITS_PER_WORD, &bits)==-1){
      perror("SPI: Can't get bits per word.");
      return -1;
   }
   if (ioctl(file, SPI_IOC_WR_MAX_SPEED_HZ, &speed)==-1){
      perror("SPI: Can't set max speed HZ");
      return -1;
   }
   if (ioctl(file, SPI_IOC_RD_MAX_SPEED_HZ, &speed)==-1){
      perror("SPI: Can't get max speed HZ.");
      return -1;
   }

   mode = mode|= SPI_3WIRE;
   printf("spi mode: 0x%x\n", mode);
   printf("bits per word: %d\n", bits);
   printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

   if (ioctl(file, SPI_IOC_MESSAGE(1), &transfer)<0){
      perror("Failed to send SPI message");
      return -1;
   }
   printf("Return value: %d\n",receive[3]);
   close(file);
   return 0;
}
