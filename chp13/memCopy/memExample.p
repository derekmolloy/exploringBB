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
#define TEST_VALUE	0xEBB00000
#define LINUX_ADDR	0x96800000
#define NUMBER_VALUES   2000000


START:
	CLR	r30.t5
        LBCO    r0, C4, 4, 4
        CLR     r0, r0, 4
        SBCO    r0, C4, 4, 4
	MOV	r1, LINUX_ADDR
	MOV	r2, TEST_VALUE
	MOV	r3, NUMBER_VALUES
	SET	r30.t5
	CLR	r30.t5

MAINLOOP:
	SUB	r3, r3, 1		// decrement r3 by 1 each loop
	SBBO	r2, r1, 0, 4            // store r2 to the address stored in r1
	ADD	r1, r1, 4
	ADD	r2, r2, 1
	QBNE	MAINLOOP, r3, 0         // keep looping until finished	
END:
	SET	r30.t5

	MOV	R31.b0, PRU0_R31_VEC_VALID | PRU_EVTOUT_0
	HALT                     // halt the pru program
