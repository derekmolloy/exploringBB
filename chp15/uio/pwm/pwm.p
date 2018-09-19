// PRUSS program to output a simple PWM signal at fixed sample rate (100)
// Writen by Derek Molloy for the book Exploring BeagleBone
// Output is r30.5 (P9_27) and Input is r31.3 (P9_28)

.origin 0               // offset of start of program in PRU memory
.entrypoint START       // program entry point used by the debugger

#define PRU0_R31_VEC_VALID 32
#define EVENTOUT0 3

START:
        // Reading the memory that was set by the C program into registers
	// r1 - Read the PWM percent high (0-100)
	MOV	r0, 0x00000000	   //load the memory location
	LBBO	r1, r0, 0, 4	   //load the percent value into r1
	// r2 - Load the sample time delay
	MOV	r0, 0x00000004	   //load the memory location
	LBBO	r2, r0, 0, 4	   //load the step delay value into r2
	// r3 - The PWM precent that the signal is low (100-r1)
	MOV	r3, 100		   //load 100 into r3
	SUB	r3, r3, r1	   //subtract r1 (high) away from 100

MAINLOOP:
	MOV	r4, r1		   // start counter at number of steps high
	SET	r30.t5	           // set the output P9_27 high
SIGNAL_HIGH:
	MOV	r0, r2		   // the delay step length - load r2 above
DELAY_HIGH:
	SUB	r0, r0, 1          // decrement delay loop counter
	QBNE	DELAY_HIGH, r0, 0  // repeat until step delay is done
	SUB	r4, r4, 1          // the signal was high for a step
	QBNE	SIGNAL_HIGH, r4, 0 // repeat until signal high steps are done

	// Now the signal is going to go low for 100%-r1% - i.e. r3
	MOV	r4, r3		   // number of steps low loaded
	CLR	r30.t5	           // set the output P9_27 low
SIGNAL_LOW:
	MOV	r0, r2		   // the delay step lenght - load r2 above
DELAY_LOW:
	SUB	r0, r0, 1          // decrement loop counter
	QBNE	DELAY_LOW, r0, 0   // repeat until step delay is done
	SUB	r4, r4, 1          // the signal was low for a step
	QBNE	SIGNAL_LOW, r4, 0  // repeat until signal low % is done

	QBBS	END, r31.t3        // quit if button on P9_28 is pressed
	QBA	MAINLOOP           // otherwise loop forever
END:                               // end of program, send back interrupt
	MOV	R31.b0, PRU0_R31_VEC_VALID | EVENTOUT0
	HALT
