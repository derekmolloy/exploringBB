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
#define TEMP_ADDR       0x9f480000
#define TEMP_SIZE	0x00040000

START:
//    	CLR	r30.t5           // timing test code -- ignore

        // Enable the OCP master port
        LBCO    r0, C4, 4, 4     // load SYSCFG reg into r0 (use c4 const addr)
        CLR     r0, r0, 4        // clear bit 4 (STANDBY_INIT)
        SBCO    r0, C4, 4, 4     // store the modified r0 back at the load addr
	
        MOV     r0, 0x00000000   // the address from which to load the address
        LBBO    r1, r0, 0, 4     // load the Linux address that is passed
        LBBO	r2, r0, 4, 4	 // load the size that is passed
//      MOV	r1, TEMP_ADDR    // for testing only -- ignore
//	MOV	r2, TEMP_SIZE    // for testing only -- ignore
	MOV	r3, TEST_VALUE   // load the data value to write to all addresses
//	SET	r30.t5		 // timing test code -- ignore
//	CLR	r30.t5           // timing test code -- ignore

MAINLOOP:
	SUB	r2, r2, 4	 // decrement counter by 4 bytes each loop
	SBBO	r3, r1, 0, 4     // store EBB value to the address stored in r1
	ADD	r1, r1, 4        // increment the write address by 4 bytes
	ADD	r3, r3, 1        // increment the EBB value by 1
	QBNE	MAINLOOP, r2, 0  // keep looping until write memory filled
END:
//	SET	r30.t5           // timing test code -- ignore
	MOV	R31.b0, PRU0_R31_VEC_VALID | PRU_EVTOUT_0
	HALT                     // halt the pru program
