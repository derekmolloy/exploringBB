// PRU program to communicate to the MCPXXXX family of SPI ADC ICs. The program 
// generates the SPI signals that are required to receive samples. To use this 
// program as is, use the following wiring configuration:
//   Chip Select (CS):   P9_27    pr1_pru0_pru_r30_5  r30.t5
//   MOSI            :   P9_29    pr1_pru0_pru_r30_1  r30.t1
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
#define TIME_CLOCK      12       // T_hi and t_lo = 125ns = 25 instructions (min)

START:
        // Enable the OCP master port -- allows transfer of data to Linux userspace
	LBCO    r0, C4, 4, 4     // load SYSCFG reg into r0 (use c4 const addr)
	CLR     r0, r0, 4        // clear bit 4 (STANDBY_INIT)
	SBCO    r0, C4, 4, 4     // store the modified r0 back at the load addr

	MOV	r1, 0x00000000	 // load the base address into r1
				 // PRU memory 0x00 stores the SPI command - e.g., 0x01 0x80 0x00
				 // the SGL/DIFF and D2, D1, D0 are the four LSBs of byte 1 - e.g. 0x80
	MOV	r7, 0x000003FF	 // the bit mask to use on the returned data (i.e., keep 10 LSBs only)
	LBBO    r8, r1, 4, 4     // load the Linux address that is passed into r8 -- to store sample values
	LBBO	r9, r1, 8, 4	 // load the size that is passed into r9 -- the number of samples to take

	MOV	r3, 0x00000000	 // clear r3 to receive the response from the MCP3XXX
	CLR	r30.t1		 // clear the data out line - MOSI

GET_SAMPLE:			 // load the send value on each sample, allows sampling re-configuration
	LBBO	r2, r1, 0, 4	 // the MCP3XXX states are now stored in r2 -- need the 16 MSBs

	// Need to wait at this point until it is ready to take a sample - i.e., 0x00010000 
	// store the address in r5
	MOV	r5, 0x00010000   // LSB of value at this address is the clock flag
SAMPLE_WAIT_HIGH:		 // wait until the PRU1 sample clock goes high
	LBBO	r6, r5, 0, 4	 // load the value at address r5 into r6		
	QBNE	SAMPLE_WAIT_HIGH, r6, 1 // if the 

	CLR	r30.t5		 // set the CS line low (active low)
	MOV	r4, 24		 // going to write/read 24 bits (3 bytes)
SPICLK_BIT:                      // loop for each of the 24 bits
	SUB	r4, r4, 1        // count down through the bits
	CALL	SPICLK           // repeat call the SPICLK procedure until all 24-bits written/read
	QBNE	SPICLK_BIT, r4, 0

	SET	r30.t5		 // pull the CS line high (end of sample)
	LSR	r3, r3, 1        // SPICLK shifts left too many times left, shift right once
	AND	r3, r3, r7	 // AND the data with mask to give only the 10 LSBs
	//SBBO	r3, r1, 12, 4    // store the data for debugging only -- REMOVE

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


// This procedure applies an SPI clock cycle to the SPI clock and on the rising edge of the clock
// it writes the current MSB bit in r2 (i.e. r31) to the MOSI pin. On the falling edge, it reads
// the input from MISO and stores it in the LSB of r3. 
// The clock cycle is determined by the datasheet of the product where TIME_CLOCK is the
// time that the clock must remain low and the time it must remain high (assuming 50% duty cycle)
// The input and output data is shifted left on each clock cycle

SPICLK:
	MOV	r0, TIME_CLOCK	 // time for clock low -- assuming clock low before cycle
CLKLOW:	
	SUB	r0, r0, 1	 // decrement the counter by 1 and loop (next line)
	QBNE	CLKLOW, r0, 0	 // check if the count is still low				 
	QBBC	DATALOW, r2.t31  // The write state needs to be set right here -- bit 31 shifted left
	SET	r30.t1
	QBA	DATACONTD
DATALOW:
	CLR	r30.t1
DATACONTD:
	SET	r30.t2		 // set the clock high
	MOV	r0, TIME_CLOCK	 // time for clock high
CLKHIGH:
	SUB	r0, r0, 1	 // decrement the counter by 1 and loop (next line)
	QBNE	CLKHIGH, r0, 0	 // check the count
	LSL	r2, r2, 1
				 // clock goes low now -- read the response on MISO
	CLR	r30.t2		 // set the clock low
	QBBC	DATAINLOW, r31.t3
	OR	r3, r3, 0x00000001
DATAINLOW:	
	LSL	r3, r3, 1 
	RET
