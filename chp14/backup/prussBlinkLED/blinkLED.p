// PRUSS program to flash a LED on GPIO1_17 gpio49 on P9_19 at 10Hz
// Writen by Derek Molloy for the book Exploring BeagleBone

.origin 0               // offset of start of program in PRU memory
.entrypoint START       // program entry point used by the debugger

// See the section on Memory-based GPIO switching in Chapter 6 - these
// addresses allowed memory based access to the GPIOs for unsafe GPIO
// switching at fast rates. The PRU will do that role here - still unsafe!

#define GPIO0 0x44e07000
#define GPIO1 0x4804c000            // only need this bank for GPIO1_17
#define GPIO2 0x481ac000
#define GPIO3 0x481ae000
#define GPIO_CLEARDATAOUT 0x190     // for clearing the registers
#define GPIO_SETDATAOUT   0x194     // for setting the registers
#define GPIO_DATAOUT      0x13c     // for reading the data
#define GPIO1_17 0x00020000         // hard-coded GPIO1_17

#define DELAY_MSEC 50               // blink every 100ms
#define CLOCKFREQ 200000000         // PRU clock frequency is 200 MHz
#define CLOCKS_PER_DELAY_LOOP 2     // loop contains two instructions, one clock each
#define DELAYCOUNT DELAY_MSEC * (CLOCKFREQ / (CLOCKS_PER_DELAY_LOOP * 1000))

// Signal to the host that the program is complete by setting bit 5 in R31
// putting the signal number in R31 bits 0-3 (5.2.2 in AM335x PRU-ICSS Reference)
#define PRU0_R31_VEC_VALID (1<<5)
#define SIGNUM 3 // corresponds to PRU_EVTOUT_0
#define NUMBER_LOOPS 100

START:
	LBCO	r0, C4, 4, 4      // Enable the PRU to access non-PRU space
	CLR	r0, r0, 4
	SBCO	r0, C4, 4, 4
        MOV	r4, NUMBER_LOOPS

LEDON:  
	MOV	r2, GPIO1 | GPIO_SETDATAOUT   //the address of the GPIO1 set bank 
	MOV	r3, GPIO1_17      // put GPIO1_17 in r3
	SBBO	r3, r2, 0, 4      
	MOV	r1, DELAYCOUNT    // store the length of the delay

DELAY:  
	SUB	r1, r1, 1         // decrement loop counter
	QBNE	DELAY, r1, 0      // repeat loop unless zero

LEDOFF: 
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT
	SBBO	r3, r2, 0, 4      // r3 contains GPIO1_17 still
	MOV	r1, DELAYCOUNT    // store the length of the delay

DELAY2:
	SUB	r1, r1, 1         // decrement loop counter
	QBNE	DELAY2, r1, 0     // repeat loop unless zero

	SUB	r4, r4, 1
        QBNE	LEDON, r4, 0      // finished looping?

        // notify calling program that it is finished
	MOV	R31.b0, PRU0_R31_VEC_VALID | SIGNUM
	HALT
