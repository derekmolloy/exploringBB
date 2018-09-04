// PRUSS program to flash a LED on P9_27 (pru0_pru_r30_5) until a button 
// that is connected to P9_28 (pru0_pru_r31_3 is pressed. This program 
// was writen by Derek Molloy for the book Exploring BeagleBone

.origin 0                        // start of program in PRU memory
.entrypoint START                // program entry point (for a debugger)

#define DELAY 4 

START:
	SET	r30.t5           // turn on the output pin (LED on)
	MOV	r0, DELAY        // store the length of the delay in REG0
DELAYON:
	SUB	r0, r0, 1        // Decrement REG0 by 1
	QBNE	DELAYON, r0, 0   // Loop to DELAYON, unless REG0=0
LEDOFF: 
	CLR	r30.t5           // clear the output bin (LED off)
	MOV	r0, DELAY        // Reset REG0 to the length of the delay
DELAYOFF:
	SUB	r0, r0, 1        // decrement REG0 by 1
	QBNE	DELAYOFF, r0, 0  // Loop to DELAYOFF, unless REG0=0
	QBBC	START, r31.t3    // is the button pressed? If not, loop
END:                             // notify the calling app that finished
	MOV	R31.b0, 35
	HALT                     // halt the pru program
