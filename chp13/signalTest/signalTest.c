#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define PRU_NUM 	0

int main (void)
{
   if(getuid()!=0){
      printf("You must run this program as root. Exiting.\n");
      exit(EXIT_FAILURE);
   }
   tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
   prussdrv_init ();
   prussdrv_open (PRU_EVTOUT_0);
   prussdrv_pruintc_init(&pruss_intc_initdata);
   prussdrv_exec_program (PRU_NUM, "./signalTest.bin");
   /* Wait for event completion from PRU */
   int n = prussdrv_pru_wait_event (PRU_EVTOUT_0);
   printf("PRU test completed, event number %d.\n", n);
   /* Disable PRU and close memory mappings */
   prussdrv_pru_disable(PRU_NUM);
   prussdrv_exit ();
   return EXIT_SUCCESS;
}
