// PRUSS program to flash a LED on P9_27 (pru0_pru_r30_5) until a button 
// that is connected to P9_28 (pru0_pru_r31_3 is pressed. This program 
// was writen by Derek Molloy for the book Exploring BeagleBone

.origin 0                        // start of program in PRU memory
.entrypoint START                // program entry point (for a debugger)

#define INS_PER_US   200         // 5ns per instruction
#define INS_PER_DELAY_LOOP 2     // two instructions per delay loop
                                 // set up a 50ms delay
#define DELAY  50 * 1000 * (INS_PER_US / INS_PER_DELAY_LOOP) 

#define PRU0_R31_VEC_VALID 32    // allows notification of program completion
#define PRU_EVTOUT_0    3        // the event number that is sent back

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
MEMORYTEST:
	MOV	r1, 0x25	 // set r1 = 0x25 = 37 (dec)
	MOV	r2, 0b100        // set r2 = 100 (binary) = 4 (dec)
	ADD	r1, r1, 5        // set r1 = r1 + 5 = 42 (dec)
	ADD	r2, r2, 1<<4     // set r2 = r2 + 10000 (bin) = 20 (dec)
	ADD	r1, r2, r1.w0    // set r1 = r2 + r1.w0 = 20 + 42 = 62 (dec)
        MOV	r0, 0x00002000   // place PRU1 data RAM1 base address in r0
        SBBO	r1, r0, 4, 4	 // write r1 to address stored in r0, offet 4

	MOV	R31.b0, PRU0_R31_VEC_VALID | PRU_EVTOUT_0
	HALT                     // halt the pru program
