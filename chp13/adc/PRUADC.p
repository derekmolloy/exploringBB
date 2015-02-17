// PRU program to communicate to the MCPXXXX family of SPI ADC ICs. The program 
// generates the SPI signals that are required to receive samples. To use this 
// program as is, use the following wiring configuration:
//   Chip Select (CS):   P9_27    pr1_pru0_pru_r30_5  r30.t5
//   MOSI            :   P9_29    pr1_pru0_pru_r30_1  r30.t1
//   MISO            :   P9_28    pr1_pru0_pru_r31_3  r31.t3
//   CLK             :   P9_30    pr1_pru0_pru_r30_2  r30.t2
//   Sample Clock    :   P8_46    pr1_pru1_pru_r30_1  -- for testing only
// This program was writen by Derek Molloy to align with the content of the book 
// Exploring BeagleBone

.setcallreg  r29.w2		 // set a non-default CALL/RET register
.origin 0                        // start of program in PRU memory
.entrypoint START                // program entry point (for a debugger)

#define PRU0_R31_VEC_VALID 32    // allows notification of program completion
#define PRU_EVTOUT_0    3        // the event number that is sent back

#define CLK_PERIOD      1000     // half of the clock period

START:
	MOV	r1, 0x00000000	 // load the base address into r1
	LBBO	r2, r1, 0, 4	 // the clock delay is now loaded into r2. 4 bytes.
	CLR	r30.t2		 // test - clear the CLK
	CLR	r30.t1		 // test - clear the MOSI - data out
	CLR	r30.t5		 // test - clear the CS line
	CALL	TOGGLECLOCK	 // move to comparison -- avoids duplicating code
	CALL	TOGGLECLOCK	 // move to comparison -- avoids duplicating code
	SET	r30.t5		 // bring CS back high
	QBA	END


TOGGLECLOCK:
	CLR	r30.t2		 // set the clock low
	MOV	r0, CLK_PERIOD	 // time for clock low
CLKLOW:
	SUB	r0, r0, 1	 // decrement the counter by 1 and loop (next line)
	QBNE	CLKLOW, r0, 0	 // check if the count is still low
	SET	r30.t2		 // set the clock low
	MOV	r0, CLK_PERIOD	 // time for clock low
CLKHIGH:
	SUB	r0, r0, 1	 // decrement the counter by 1 and loop (next line)
	QBNE	CLKHIGH, r0, 0	 // check if the count is still low
	RET

END:
	MOV	r31.b0, PRU0_R31_VEC_VALID | PRU_EVTOUT_0	
	HALT                     // halt the pru program -- never reached






