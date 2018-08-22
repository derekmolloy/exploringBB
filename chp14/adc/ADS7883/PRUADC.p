// PRU program to communicate to the ADS7883 family of SPI ADC ICs. To use this 
// program as is, use the following wiring configuration:
//   Chip Select (CS):   P9_27    pr1_pru0_pru_r30_5  r30.t5
//   MISO            :   P9_28    pr1_pru0_pru_r31_3  r31.t3
//   CLK             :   P9_30    pr1_pru0_pru_r30_2  r30.t2
//   Sample Clock    :   P8_46    pr1_pru1_pru_r30_1  -- for testing only
// This program was writen by Derek Molloy to align with the content of the book 
// Exploring BeagleBone. See exploringbeaglebone.com/chapter13/

.setcallreg  r29.w2		 // set a non-default CALL/RET register
.origin 0                        // start of program in PRU memory
.entrypoint START                // program entry point (for a debugger)

#define PRU0_R31_VEC_VALID 32    // allows notification of program completion
#define PRU_EVTOUT_0    3        // the event number that is sent back

// Constants from the MCP3004/3008 datasheet 
#define TIME_CLOCK      1       // T_hi and t_lo = 125ns = 25 instructions (min)

START:
        // Enable the OCP master port -- allows transfer of data to Linux userspace
	LBCO    r0, C4, 4, 4     // load SYSCFG reg into r0 (use c4 const addr)
	CLR     r0, r0, 4        // clear bit 4 (STANDBY_INIT)
	SBCO    r0, C4, 4, 4     // store the modified r0 back at the load addr

	MOV	r1, 0x00000000	 // load the base address into r1
				 // PRU memory 0x00 stores the SPI command - e.g., 0x01 0x80 0x00
				 // the SGL/DIFF and D2, D1, D0 are the four LSBs of byte 1 - e.g. 0x80
	MOV	r7, 0x00000FFF	 // the bit mask to use on the returned data (i.e., keep 10 LSBs only)
	LBBO    r8, r1, 4, 4     // load the Linux address that is passed into r8 -- to store sample values
	LBBO	r9, r1, 8, 4	 // load the size that is passed into r9 -- the number of samples to take

	MOV	r3, 0x00000000	 // clear r3 to receive the response from the MCP3XXX
	CLR	r30.t2		 // set the clock low

GET_SAMPLE:
	// Need to wait at this point until it is ready to take a sample - i.e., 0x00010000 
	// store the address in r5
	MOV	r5, 0x00010000   // LSB of value at this address is the clock flag
SAMPLE_WAIT_HIGH:		 // wait until the PRU1 sample clock goes high
	LBBO	r6, r5, 0, 4	 // load the value at address r5 into r6		
	QBNE	SAMPLE_WAIT_HIGH, r6, 1 // wait until the sample clock goes high

	CLR	r30.t5		 // set the CS line low (active low)
	MOV	r4, 16		 // going to write/read 16 bits (2 bytes)
SPICLK_BIT:                      // loop for each of the 16 bits
	SUB	r4, r4, 1        // count down through the bits
// *************************************
	LSL	r3, r3, 1        // shift the captured data left by one position 
	// Clock goes low for a time period
	SET	r30.t2		 // set the clock high
	SET	r30.t2		 // set the clock high
	SET	r30.t2		 // set the clock high
	QBBC	DATAINLOW, r31.t3   // check if the bit that is read in is low? jump
	OR	r3, r3, 0x00000001  // set the stored bit LSB to 1 otherwise
DATAINLOW:	                 
	CLR	r30.t2		 // set the clock low
	CLR	r30.t2		 // set the clock low
// **************************************
	QBNE	SPICLK_BIT, r4, 0  // have we performed 16 cycles?

	LSR	r3, r3, 2        // SPICLK shifts left too many times left, shift right once
	AND	r3, r3, r7	 // AND the data with mask to give only the 10 LSBs
	SET	r30.t5		 // pull the CS line high (end of sample)

STORE_DATA:                      // store the sample value in memory
	SUB	r9, r9, 2	 // reducing the number of samples - 2 bytes per sample
	SBBO	r3.w0, r8, 0, 2	 // store the value r3 in memory
	ADD	r8, r8, 2	 // shifting by 2 bytes - 2 bytes per sample
	QBEQ	END, r9, 0       // have taken the full set of samples

SAMPLE_WAIT_LOW:                 // need to wait here if the sample clock has not gone low
	LBBO	r6, r5, 0, 4	 // load the value in PRU1 sample clock address r5 into r6
	QBNE	SAMPLE_WAIT_LOW, r6, 0 // wait until the sample clock goes low (just in case)
	QBA	GET_SAMPLE
END:
	MOV	r31.b0, PRU0_R31_VEC_VALID | PRU_EVTOUT_0	
	HALT                     // End of program -- below are the "procedures"


