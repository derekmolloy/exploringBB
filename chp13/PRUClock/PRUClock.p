// PRUSS program to provide a variable frequency clock on P9_27 (pru0_pru_r30_5) 
// that is controlled from Linux userspace by setting the PRU memory state. This
// program can be executed on either PRU0 or PRU1, but will have to be altered if
// you wish to change the clock to output on a different pin.
// The delay is set in memory address 0x00000000 (4 bytes)
// The counter can be turned on and off by setting 0x00000004 (4 bytes) to 1 (on) or 0 (off)
// This program was writen by Derek Molloy for the book Exploring BeagleBone

.origin 0                        // start of program in PRU memory
.entrypoint START                // program entry point (for a debugger)

#define PRU0_R31_VEC_VALID 32    // allows notification of program completion
#define PRU_EVTOUT_0    3        // the event number that is sent back

START:
	// Load the clock period value into r2
	MOV	r1, 0x00000000	 // This address contains the delay value
	LBBO	r2, r1, 0, 4	 // The clock delay is now stored in r2. 4 bytes.
	QBA	ENDOFLOOP	 // Move to comparison -- avoid duplicating code

MAINLOOP:
	CLR	r30.t5           // clock is now low
	MOV	r0, r2		 // load the delay r2 into r0 
	ADD	r0, r0, 1	 // balance the duty cycle
DELAYOFF:
	SUB	r0, r0, 1	 // decrement the counter
	QBNE	DELAYOFF, r0, 0  // loop until the delay has expired
				 // code order used to help set 50% duty cycle
	MOV	r0, r2           // re-load the delay r2 into r0
	SET	r30.t5		 // clock is now high
DELAYON:
	SUB	r0, r0, 1	 // decrement the counter
	QBNE	DELAYON, r0, 0   // loop until the delay has expired

ENDOFLOOP:                       // Is the clock running? 
	LBBO	r3, r1, 4, 4	 // Loaded in the state into r3 -- is running? 4 bytes.
	QBBS	RESETCLK, r3.t1	 // If r3 bit 1 is high then reload the clock period
	QBBS	MAINLOOP, r3.t0  // If bit 0 is high then the clock is running
	QBA	ENDOFLOOP        // otherwise loop without changing the clock state

RESETCLK:			 // clear the r3.t1 bit and write back to memory
	CLR	r3, r3.t1	 // clear the reload clock flag
	SBBO	r3, r1, 4, 4     // write that value back into memory
	QBA	START            // go back to the start of the program
END:				 // program will not exit
	MOV	R31.b0, PRU0_R31_VEC_VALID | PRU_EVTOUT_0
	HALT                     // halt the pru program
