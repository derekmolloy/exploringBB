// PRUSS program to output a waveform that is passed from the C program
// Writen by Derek Molloy for the book Exploring BeagleBone
// Output is r30.5 (P9_27) and Input is r31.3 (P9_28)

.origin 0               // offset of start of program in PRU memory
.entrypoint START       // program entry point used by the debugger

#define PRU0_R31_VEC_VALID 32
#define EVENTOUT0 3

START:
        // Reading the memory that was set by the C program into registers
	// r2 - Load the sample time delay
	MOV	r0, 0x00000000	   //load the memory location
	LBBO	r2, r0, 0, 4	   //load the step delay value into r2
	// r5 - Load the number of samples
	MOV	r0, 0x00000004	   //load the memory location
	LBBO	r5, r0, 0, 4	   //load the number of samples into r5
	MOV	r6, 0	           //r6 is the counter 0 to r5
	MOV	r7, 0x00000008	   //load the memory location

SAMPLELOOP:
	//Is the counter r6  >= the number of samples r5, if so go back to 0
	//If r6 < r5 then just continue, else set back to 0

        QBLT	CONTINUE, r5, r6 
        MOV	r7, 0x00000008
        MOV	r6, 0	           //loop all over again
CONTINUE:
	// r1 - Read the PWM percent high (0-100)
	ADD	r7, r7, 4 
	LBBO	r1, r7, 0, 4	   //load the percent value into r1

	//check that the value is between 1 and 99
	QBLT	GREATERTHANZERO, r1, 0
        MOV	r1, 1
GREATERTHANZERO:
        QBGT	LESSTHAN100, r1, 100
	MOV	r1, 99
LESSTHAN100:

	// r3 - The PWM precent that the signal is low (100-r1)
	MOV	r3, 0x64           //load 100 into r3
	SUB	r3, r3, r1	   //subtract r1 (high) away.

	// This is for a single sample:
	MOV	r4, r1		   // number of steps high
	SET	r30.t5	           // set the output P9_27 high
SIGNAL_HIGH:
	MOV	r0, r2		   // delay how long? load r2 above
DELAY_HIGH:
	SUB	r0, r0, 1          // decrement loop counter
	QBNE	DELAY_HIGH, r0, 0  // repeat until step delay is done
	SUB	r4, r4, 1          // the signal was high for a step
	QBNE	SIGNAL_HIGH, r4, 0 // repeat until signal high % is done

	// Now the signal is going to go low for 100%-r1% - i.e. r3
	MOV	r4, r3		   // number of steps low loaded
	CLR	r30.t5	           // set the output P9_27 low
SIGNAL_LOW:
	MOV	r0, r2		   // delay how long? load r2 above
DELAY_LOW:
	SUB	r0, r0, 1          // decrement loop counter
	QBNE	DELAY_LOW, r0, 0   // repeat until step delay is done
	SUB	r4, r4, 1          // the signal was low for a step
	QBNE	SIGNAL_LOW, r4, 0  // repeat until signal low % is done

	QBBS	END, r31.t3        // quit if button on P9_28 is pressed
	// Go to the next sample
	ADD	r6, r6, 1	   // increment the counter by 1
	QBA	SAMPLELOOP         // otherwise loop forever

END:                               // end of program, send back interrupt
	MOV	R31.b0, PRU0_R31_VEC_VALID | EVENTOUT0
	HALT
