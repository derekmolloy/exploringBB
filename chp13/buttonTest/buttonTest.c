/* A program to load the buttonTest.bin code into PRU0
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <stdio.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define PRU_NUM 	0

void main (void)
{
   tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
   prussdrv_init ();
   prussdrv_open (PRU_EVTOUT_0);
   prussdrv_pruintc_init(&pruss_intc_initdata);
   prussdrv_exec_program (PRU_NUM, "./buttonTest.bin");
   /* Wait for event completion from PRU */
   int n = prussdrv_pru_wait_event (PRU_EVTOUT_0);
   printf("PRU test completed, event number %d.\n", n);
   /* Disable PRU and close memory mappings */
   prussdrv_pru_disable(PRU_NUM);
   prussdrv_exit ();
}
