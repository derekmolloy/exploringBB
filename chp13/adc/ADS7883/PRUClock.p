// PRU1 program to provide a variable frequency clock on P8_46 (pru1_pru_r30_1) 
// that is controlled from Linux userspace by setting the PRU memory state. This
// program is executed on PRU1 and outputs the sample clock on P8_46. If you wish
// to change the clock to output on a different pin edit the code. The program is
// memory controlled using the first two 4-byte numbers in PRU memory space:
// - The delay is set in memory address 0x00000000 (4 bytes long)
// - The counter can be turned on and off by setting the LSB of 0x00000004 (4 bytes) 
//   to 1 (on) or 0 (off)
// - The delay value can be updated by setting the second most LSB to 1 (it will 
//   immediately return to 1) e.g., set 0x00000004 to be 3, which will return to 1
//   to indicate that the update has been performed.
// This program was writen by Derek Molloy to align with the content of the book 
// Exploring BeagleBone, 2014. This example was written after publication.

.origin 0                        // start of program in PRU memory
.entrypoint START                // program entry point (for a debugger)

START:	                         // aims to load the clock period value into r2
	MOV	r1, 0x00000000	 // load the base address into r1
	LBBO	r2, r1, 0, 4	 // the clock delay is now loaded into r2. 4 bytes.
	MOV	r4, 0x00010000   // Going to use PRU shared memory to share the state change
	MOV	r5, r1		 // r5 going to store the state of the clock i.e. high/low
	QBA	ENDOFLOOP	 // move to comparison -- avoids duplicating code

MAINLOOP:
	CLR	r30.t1           // set the sample clock signal to be low
	CLR	r5.t00
	SBBO    r5, r4, 0, 4	 // store the clock state in PRU shared memory
	MOV	r0, r2		 // load the delay r2 into temp r0 (50% duty cycle)
	ADD	r0, r0, 1	 // balance duty cycle by looping 1 extra time on low

DELAYOFF:
	SUB	r0, r0, 1	 // decrement the counter by 1 and loop (next line)
	QBNE	DELAYOFF, r0, 0  // loop until the delay has expired (i.e., equals 0)
				 // next instruction order used to keep 50% duty cycle
	MOV	r0, r2           // re-load the delay r2 into temporary r0
	SET	r30.t1		 // set the sample clock to be high
	SET	r5.t00
	SBBO    r5, r4, 0, 4	 // store the clock state in PRU shared memory

DELAYON:
	SUB	r0, r0, 1	 // decrement the counter by 1 and loop (next line)
	QBNE	DELAYON, r0, 0   // loop until the delay has expired (equals 0)

ENDOFLOOP:                       // is the clock running? 
	LBBO	r3, r1, 4, 4	 // loaded the state into r3 -- is running? 4 bytes total
	QBBS	RESETCLK, r3.t1	 // If r3 bit 1 is high then reload the clock period
	QBBS	MAINLOOP, r3.t0  // If r3 bit 0 is high then the clock is running
	QBA	ENDOFLOOP        // otherwise loop without toggling the clock -- i.e. clock off

RESETCLK:			 // clear the r3.t1 bit and write back to memory
				 // indicating that the clock frequency has been updated
	CLR	r3, r3.t1	 // i.e., clear the reload clock flag
	SBBO	r3, r1, 4, 4     // write that value back into memory
	QBA	START            // go back to the start of the program

END:				 // program will not exit due to the QBA on the line above
	HALT                     // halt the pru program -- never reached!
