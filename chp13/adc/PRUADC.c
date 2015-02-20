/** Program to load a PRU program that flashes an LED until a button is
*   pressed. By Derek Molloy, for the book Exploring BeagleBone
*   based on the example code at:
*   http://processors.wiki.ti.com/index.php/PRU_Linux_Application_Loader_API_Guide
*/

#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define ADC_PRU_NUM	   0   // using PRU0 for the ADC capture
#define CLK_PRU_NUM	   1   // using PRU1 for the sample clock
#define MMAP0_LOC   "/sys/class/uio/uio0/maps/map0/"
#define MMAP1_LOC   "/sys/class/uio/uio0/maps/map1/"

enum FREQUENCY {    // measured and calibrated, but can be calculated
	FREQ_12_5MHz =  1,
	FREQ_6_25MHz =  5,
	FREQ_5MHz    =  7,
	FREQ_3_85MHz = 10,
	FREQ_1MHz   =  45,
	FREQ_500kHz =  95,
	FREQ_250kHz = 245,
	FREQ_100kHz = 495,
	FREQ_25kHz = 1995,
	FREQ_10kHz = 4995,
	FREQ_5kHz =  9995,
	FREQ_2kHz = 24995,
	FREQ_1kHz = 49995
};

enum CONTROL {
	PAUSED = 0,
	RUNNING = 1,
	UPDATE = 3
};

unsigned int readFileValue(char filename[]){
   FILE* fp;
   unsigned int value = 0;
   fp = fopen(filename, "rt");
   fscanf(fp, "%x", &value);
   fclose(fp);
   return value;
}

int main (void)
{
   if(getuid()!=0){
      printf("You must run this program as root. Exiting.\n");
      exit(EXIT_FAILURE);
   }
   // Initialize structure used by prussdrv_pruintc_intc
   // PRUSS_INTC_INITDATA is found in pruss_intc_mapping.h
   tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

   // Read in the location and address of the shared memory. This value changes
   // each time a new block of memory is allocated.
   unsigned int timerData[2];
   timerData[0] = FREQ_100kHz;
   timerData[1] = RUNNING;
   printf("The clock state is set as period: %d (0x%x) and state: %d\n", timerData[0], timerData[0], timerData[1]);
   unsigned int PRU_data_addr = readFileValue(MMAP0_LOC "addr");
   printf("The PRU memory is mapped at the base address: %x\n", (PRU_data_addr + 0x2000));
   printf("The clock on/off state is mapped at address: %x\n", (PRU_data_addr + 0x10000));

   // data for PRU0 based on the MCPXXXX datasheet
   unsigned int spiData[3];
   spiData[0] = 0x01800000;
   spiData[1] = readFileValue(MMAP1_LOC "addr");
   spiData[2] = readFileValue(MMAP1_LOC "size");
   printf("Sending the SPI Control Data: 0x%x\n", spiData[0]);
   printf("The Shared memory has location: 0x%x and size 0x%x\n", spiData[1], spiData[2]);

   // Allocate and initialize memory
   prussdrv_init ();
   prussdrv_open (PRU_EVTOUT_0);

   // Write the address and size into PRU0 Data RAM0. You can edit the value to
   // PRUSS0_PRU1_DATARAM if you wish to write to PRU1
   prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, spiData, 12);  // spi code
   prussdrv_pru_write_memory(PRUSS0_PRU1_DATARAM, 0, timerData, 8); // sample clock

   // Map PRU's interrupts
   prussdrv_pruintc_init(&pruss_intc_initdata);

   // Load and execute the PRU program on the PRU
   prussdrv_exec_program (ADC_PRU_NUM, "./PRUADC.bin");
   prussdrv_exec_program (CLK_PRU_NUM, "./PRUClock.bin");
   printf("EBB Clock PRU1 program now running (%d)\n", timerData[0]);

   // Wait for event completion from PRU, returns the PRU_EVTOUT_0 number
   int n = prussdrv_pru_wait_event (PRU_EVTOUT_0);
   printf("EBB ADC PRU program completed, event number %d.\n", n);

// Disable PRU and close memory mappings 
   prussdrv_pru_disable(ADC_PRU_NUM);
   prussdrv_pru_disable(CLK_PRU_NUM);

   prussdrv_exit ();
   return EXIT_SUCCESS;
}
