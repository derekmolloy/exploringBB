// PRUSS program to turn off an LED if a button is pressed - speed test
//  LED output   - pru1_pru0_pru_r30_5 mode=5   P9_27
//  Button input - pru1_pru0_pru_r31_3 mode=6   P9_28
// Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
// Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
// ISBN 9781118935125. Please see the file README.md in the repository root 
// directory for copyright and GNU GPLv3 license information.     

.origin 0             
.entrypoint START   

#define PRU0_R31_VEC_VALID 32
#define PRU_EVTOUT_0 3

START:
        CLR	r30.t5      // turn off the LED
	WBS	r31.t3      // wait bit set - i.e. button press
	SET	r30.t5      // set the output bit - turn on the LED
	MOV	r31.b0, 35
	HALT
