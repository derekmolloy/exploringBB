// PRUSS program to turn off an LED if a button is pressed - speed test
//  LED output - pru1_pru0_pru_r30_5 mode=5   P9_27
//  Button input - pru1_pru0_pru_r31_3 mode=6 P9_28
// Writen by Derek Molloy for the book Exploring BeagleBone

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
