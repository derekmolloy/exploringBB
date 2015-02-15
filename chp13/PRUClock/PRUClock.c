/** Program to load a PRU program that flashes an LED until a button is
*   pressed. By Derek Molloy, for the book Exploring BeagleBone
*   based on the example code at:
*   http://processors.wiki.ti.com/index.php/PRU_Linux_Application_Loader_API_Guide
*/

#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define PRU_NUM	   0   // using PRU0 for these examples
#define MMAP_LOC   "/sys/class/uio/uio0/maps/map0/"

enum FREQUENCY {    // measured and calibrated, but can be calculated
	FREQ_12_5MHz =  1,
	FREQ_6_25MHz =  5,
	FREQ_5MHz    =  7,
	FREQ_3_85MHz = 10,
	FREQ_1MHz   =  47,
	FREQ_500kHz =  97,
	FREQ_250kHz = 247,
	FREQ_100kHz = 497,
	FREQ_25kHz = 1997,
	FREQ_10kHz = 4997,
	FREQ_5kHz =  9997,
	FREQ_2kHz = 24997,
	FREQ_1kHz = 49997
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
   unsigned int values[2];
   values[0] = FREQ_1MHz;
   values[1] = RUNNING;
   printf("The clock state is set as period: %d (0x%x) and state: %d\n", values[0], values[0], values[1]);
   printf("This is mapped at the base address: %x\n", readFileValue(MMAP_LOC "addr"));

   // Allocate and initialize memory
   prussdrv_init ();
   prussdrv_open (PRU_EVTOUT_0);

   // Write the address and size into PRU0 Data RAM0. You can edit the value to
   // PRUSS0_PRU1_DATARAM if you wish to write to PRU1
   prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, values, 8);

   // Map PRU's interrupts
   prussdrv_pruintc_init(&pruss_intc_initdata);

   // Load and execute the PRU program on the PRU
   prussdrv_exec_program (PRU_NUM, "./PRUClock.bin");
   printf("EBB Clock PRU program now running (%d)\n", values[0]);

   prussdrv_exit ();
   return EXIT_SUCCESS;
}
