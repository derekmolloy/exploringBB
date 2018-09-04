// PRU program to provide a variable frequency clock on P9_27 (pru0_pru_r30_5) 
// that is controlled from Linux userspace by setting the PRU memory state. This
// program can be executed on either PRU0 or PRU1, but will have to be altered if
// you wish to change the clock to output on a different pin. Memory controlled:
// - The delay is set in memory address 0x00000000 (4 bytes)
// - The counter can be turned on and off by setting the LSB 0x00000004 (4 bytes) 
//   to 1 (on) or 0 (off)
// - The delay value can be updated by setting the second most LSB to 1 (it will 
//   immediately return to 0)
// This program was writen by Derek Molloy to align with the content of the book 
// Exploring BeagleBone

.origin 0                        // start of program in PRU memory
.entrypoint START                // program entry point (for a debugger)

START:	                         // load the clock period value into r2
	MOV	r1, 0x00000000	 // load the base address into r1
	LBBO	r2, r1, 0, 4	 // the clock delay is now loaded into r2. 4 bytes.
	QBA	ENDOFLOOP	 // move to comparison -- avoids duplicating code

MAINLOOP:
	CLR	r30.t5           // set the clock to be low
	MOV	r0, r2		 // load the delay r2 into temp r0 (50% duty cycle)
	ADD	r0, r0, 1	 // balance duty cycle by looping 1 extra time on low

DELAYOFF:
	SUB	r0, r0, 1	 // decrement the counter by 1 and loop (next line)
	QBNE	DELAYOFF, r0, 0  // loop until the delay has expired (equals 0)
				 // next code order used to help keep 50% duty cycle
	MOV	r0, r2           // re-load the delay r2 into temporary r0
	SET	r30.t5		 // set the clock to be high

DELAYON:
	SUB	r0, r0, 1	 // decrement the counter by 1 and loop (next line)
	QBNE	DELAYON, r0, 0   // loop until the delay has expired (equals 0)

ENDOFLOOP:                       // is the clock running? 
	LBBO	r3, r1, 4, 4	 // loaded in the state into r3 -- is running? 4 bytes.
	QBBS	RESETCLK, r3.t1	 // If r3 bit 1 is high then reload the clock period
	QBBS	MAINLOOP, r3.t0  // If r3 bit 0 is high then the clock is running
	QBA	ENDOFLOOP        // otherwise loop without toggling the clock

RESETCLK:			 // clear the r3.t1 bit and write back to memory
	CLR	r3, r3.t1	 // i.e., clear the reload clock flag
	SBBO	r3, r1, 4, 4     // write that value back into memory
	QBA	START            // go back to the start of the program

END:				 // program will not exit
	HALT                     // halt the pru program -- never reached
