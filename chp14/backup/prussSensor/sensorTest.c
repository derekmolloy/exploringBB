/* PRUSS program to drive a HC-SR04 sensor and display the sensor output
*  in Linux userspace.
*  written by Derek Molloy for the book Exploring BeagleBone
*/
#include <stdio.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

// Using PRU 0 for this example
#define PRU_NUM 0

static void *pru0DataMemory;
static unsigned int *pru0DataMemory_int;

void main (void)
{
   int n;
   // Initialize structure used by prussdrv_pruintc_intc
   // PRUSS_INTC_INITDATA is found in pruss_intc_mapping.h
   tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

   // Allocate and initialize memory 
   prussdrv_init ();
   prussdrv_open (PRU_EVTOUT_0);

   // Map PRU's INTC 
   prussdrv_pruintc_init(&pruss_intc_initdata);

   // Copy data to PRU memory - different way
   prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pru0DataMemory);
   pru0DataMemory_int = (unsigned int *) pru0DataMemory;
   // Use the first 4 bytes for the number of samples
   *pru0DataMemory_int = 5;
   // Use the second 4 bytes for the sample delay in ms
   *(pru0DataMemory_int+1) = 2;   // 2 seconds between samples

   // Load and execute binary on PRU 
   prussdrv_exec_program (PRU_NUM, "./sensorTest.bin");

   // Wait for event completion from PRU 
   n = prussdrv_pru_wait_event (PRU_EVTOUT_0);
   printf("PRU program completed, event number %d.\n", n);

   printf("The data that is in memory is:\n");
   printf("- the number of samples used is %d.\n", *pru0DataMemory_int);
   printf("- the time delay used is %d.\n", *(pru0DataMemory_int+1));
   unsigned int raw_distance = *(pru0DataMemory_int+2);
   printf("- the last distance sample is %d.\n", raw_distance);
   // raw_distance is in 10ns samples
   // distance in inches = time (ms) / 148 according to datasheet
   float distin = ((float)raw_distance / (100 * 148));
   float distcm = ((float)raw_distance / (100 * 58));
   printf("-- A distance of %f inches (%f cm).\n", distin, distcm);

   // Disable PRU and close memory mappings 
   prussdrv_pru_disable(PRU_NUM);
   prussdrv_exit ();
}
