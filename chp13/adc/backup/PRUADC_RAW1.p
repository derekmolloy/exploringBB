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

// Constants from the MCP3004/3008 datasheet 
#define TIME_CLOCK      13       // T_hi and t_LO = 125ns = 13 x 2 instructions (min)
#define TIME_SUCS       10       // T_sucs = 100ns = 10 x 2 instructions (min)
				 // T_sucs is min time from CS fall to first CLK edge
#define TIME_DATA_SETUP  5       // T_su = 50ns = 5 x 2 instructions
#define TIME_INPUT_HOLD  5       // T_hd = 50ns = 5 x 2 instructions
#define TIME_DISABLE    27       // T_csh = 270ns = 27 x 2 instructions

START:
	MOV	r1, 0x00000000	 // load the base address into r1
				 // the SGL/DIFF and D2, D1, D0 are the four LSBs of byte 1
	LBBO	r2, r1, 0, 1	 // the MCP3XXX states are now stored in r2

	// The code below works, but is pretty bad

SAMPLE:
	CLR	r30.t5		 // set the CS line low (active low)
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	SET	r30.t1
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CLR	r30.t1
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK
	CALL	TOGGLECLK

END_SAMPLE:
	SET	r30.t5		 // pull the CS back high (end of sample)
	QBA	END


DELAY_R0:
LOOPR0:	SUB	r0, r0, 1
	QBNE	LOOPR0, r0, 0
	RET

TOGGLECLK:
	MOV	r0, TIME_CLOCK	 // time for clock high
	CLR	r30.t2		 // set the clock low
CLKLOW:
	SUB	r0, r0, 1	 // decrement the counter by 1 and loop (next line)
	QBNE	CLKLOW, r0, 0	 // check if the count is still low
	SET	r30.t2		 // set the clock low
	MOV	r0, TIME_CLOCK	 // time for clock low
CLKHIGH:
	SUB	r0, r0, 1	 // decrement the counter by 1 and loop (next line)
	QBNE	CLKHIGH, r0, 0	 // check if the count is still low
	RET

END:
	MOV	r31.b0, PRU0_R31_VEC_VALID | PRU_EVTOUT_0	
	HALT                     // halt the pru program -- never reached






