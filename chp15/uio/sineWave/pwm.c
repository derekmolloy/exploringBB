/** Program to generate a simple PWM signal using the BBB PRUSS
*   by Derek Molloy, for the book Exploring BeagleBone
*/
#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <math.h>

#define PRU_NUM 	0
static void *pru0DataMemory;
static unsigned int *pru0DataMemory_int;

int main (void)
{
   if(getuid()!=0){
      printf("You must run this program as root. Exiting.\n");
      exit(EXIT_FAILURE);
   }
   int n, i;
   // Initialize structure used by prussdrv_pruintc_intc
   // PRUSS_INTC_INITDATA is found in pruss_intc_mapping.h
   tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

   // Allocate and initialize memory
   prussdrv_init ();
   prussdrv_open (PRU_EVTOUT_0);

   // Map PRU's INTC
   prussdrv_pruintc_init(&pruss_intc_initdata);

   // Generate a periodic sine wave in an array of 100 values - using ints
   unsigned int waveform[100];
   float gain = 50.0f;
   float phase = 0.0f;
   float bias = 50.0f;
   float freq = 2.0f * 3.14159f / 100.0f;
   for (i=0; i<100; i++){
      waveform[i] = (unsigned int)(bias + (gain * sin((i * freq) + phase)));
   }

   // place the samples in memory
   prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pru0DataMemory);
   pru0DataMemory_int = (unsigned int *) pru0DataMemory;
   unsigned int sampletimestep = 1;  //delay factor
   *(pru0DataMemory_int) = sampletimestep;
   unsigned int numbersamples = 100;  //number of samples
   *(pru0DataMemory_int+1) = numbersamples;
   // copy the waveform data into PRU memory
   for (i=0; i<numbersamples; i++){
      *(pru0DataMemory_int+2+i) = waveform[i];
   }

   // Load and execute binary on PRU
   prussdrv_exec_program (PRU_NUM, "./pwm.bin");

   // Wait for event completion from PRU
   n = prussdrv_pru_wait_event (PRU_EVTOUT_0);  // This assumes the PRU generates an interrupt
                                                    // connected to event out 0 immediately before halting
   printf("PRU program completed, event number %d.\n", n);

   // Disable PRU and close memory mappings
   prussdrv_pru_disable(PRU_NUM);
   prussdrv_exit ();
   return EXIT_SUCCESS;
}
