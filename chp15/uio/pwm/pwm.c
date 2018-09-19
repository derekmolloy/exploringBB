/** Program to generate a simple PWM signal using the BBB PRUSS
*   Output is P9_27 and input is P9_28, using PRU EGPIOs
*   by Derek Molloy, for the book Exploring BeagleBone
*/
#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define PRU_NUM   0

int main (void)
{
   if(getuid()!=0){
      printf("You must run this program as root. Exiting.\n");
      exit(EXIT_FAILURE);
   }
   // Initialize structure used by prussdrv_pruintc_intc
   tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

   // Allocate and initialize memory
   prussdrv_init ();
   prussdrv_open (PRU_EVTOUT_0);

   // Map PRU intrrupts
   prussdrv_pruintc_init(&pruss_intc_initdata);

   // Copy the PWM percentage and delay factor into PRU memory
   unsigned int percent = 75;  //(0-100)
   prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, &percent, 4);
   unsigned int sampletimestep = 10;  //delay factor
   // write it into the next word location in memory (i.e. 4-bytes later)
   prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 1, &sampletimestep, 4);

   // Load and execute binary on PRU
   prussdrv_exec_program (PRU_NUM, "./pwm.bin");

   // Wait for event completion from PRU
   int n = prussdrv_pru_wait_event (PRU_EVTOUT_0);
   printf("PRU program completed, event number %d.\n", n);

   // Disable PRU and close memory mappings
   prussdrv_pru_disable(PRU_NUM);
   prussdrv_exit ();
   return EXIT_SUCCESS;
}
