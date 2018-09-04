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

#define DELAYNUM 47  		    // acount for led on/off instructions

// Signal to the host that the program is complete by setting bit 5 in R31
// putting the signal number in R31 bits 0-3 (5.2.2 in AM335x PRU-ICSS Reference)
#define PRU0_R31_VEC_VALID (1<<5)
#define SIGNUM 3 // corresponds to PRU_EVTOUT_0

START:
	LBCO	r0, C4, 4, 4      // Enable the PRU to access non-PRU space
	CLR	r0, r0, 4
	SBCO	r0, C4, 4, 4
	MOV	r3, GPIO1_17      // put GPIO1_17 in r3

LEDON:  
	MOV	r2, GPIO1 | GPIO_SETDATAOUT   //the address of the GPIO1 set bank 
	SBBO	r3, r2, 0, 4      
	MOV	r1, DELAYNUM      // store the length of the delay

DELAY:  
	SUB	r1, r1, 1         // decrement loop counter
	QBNE	DELAY, r1, 0      // repeat loop unless zero

LEDOFF: 
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT
	SBBO	r3, r2, 0, 4      // r3 contains GPIO1_17 still
	MOV	r1, DELAYNUM      // store the length of the delay

DELAY2:
	SUB	r1, r1, 1         // decrement loop counter
	QBNE	DELAY2, r1, 0     // repeat loop unless zero

        QBA	LEDON		  // quick branch always

NEVER:
        // notify calling program that it is finished
	MOV	R31.b0, PRU0_R31_VEC_VALID | SIGNUM
	HALT
