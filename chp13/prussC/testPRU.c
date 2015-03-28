/** Basic PRU C program to flash an LED that is attached to P9_27 at a
*   frequency of 10 Hz, until a button that is attached to P9_28 is pressed
*   you must load the device tree overlay in advance
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root
* directory for copyright and GNU GPLv3 license information.            */

/* the registers for I/O and interrupts */
volatile register unsigned int __R31, __R30;

unsigned int i;                  // the counter in the time delay
unsigned int delay = 588260;     // the delay (manually determined)

int main()
{
   // Just a test to show how you can use assembly instructions directly
   // subtract 1 from REG1
   __asm__ __volatile__
   (
      " SUB r1, r1, 1 \n"
   );

   // while the button r31.3 has not been pressed, keep looping
   while(!(__R31 & 1<<3)){
      __R30 = __R30 | 1<<5;         // turn on the LED r30.5
      for(i=0; i<delay; i++) {}     // sleep for the delay
      __R30 = __R30 & 0<<5;         // turn off the LED r30.5
      for(i=0; i<delay; i++) {}     // sleep for the delay
   }

   // Exiting the application - send the interrupt
   __R31 = 35;                      // PRUEVENT_0 on PRU0_R31_VEC_VALID
   __halt();                        // halt the PRU
}
