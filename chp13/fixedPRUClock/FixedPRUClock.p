// PRU program to provide a fixed-frequency clock on P9_27 (pru0_pru_r30_5). This
// program can be executed on either PRU0 or PRU1, but will have to be altered if
// you wish to change the clock to output on a different pin. Memory controlled:
// This program was writen by Derek Molloy to align with the content of the book 
// Exploring BeagleBone

.origin 0                        // start of program in PRU memory
.entrypoint START                // program entry point (for a debugger)

#define	DELAY   498  		 // choose the delay value to suit the frequency required
				 // 1 gives a 20MHz clock signal, increase from there
START:
	MOV	r1, DELAY	 // load the DELAY value into r1

MAINLOOP:
	CLR	r30.t5		// set the clock to be low
	CLR	r30.t5		// set the clock to be low -- to balance the duty cycle
	MOV	r0, r1		// load the delay r1 into temp r0 (50% duty cycle)

DELAYOFF:
	SUB	r0, r0, 1	// decrement the counter by 1 and loop (next line)
	QBNE	DELAYOFF, r0, 0	// loop until the delay has expired (equals 0)
	SET	r30.t5		// set the clock to be high
	MOV	r0, r1		// re-load the delay r1 into temporary r0

DELAYON:
	SUB	r0, r0, 1	// decrement the counter by 1 and loop (next line)
	QBNE	DELAYON, r0, 0	// loop until the delay has expired (equals 0)

	QBA	MAINLOOP	// start again, so the program will not exit
END:
	HALT			// halt the pru program -- never reached
