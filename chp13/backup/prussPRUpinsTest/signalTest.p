// PRUSS program to toggle a GPIO P9_27 at 1 MHz
//  pru1_pru0_pru_r30_5 mode=5 offset=0x1A4
// Writen by Derek Molloy for the book Exploring BeagleBone

.origin 0             
.entrypoint START   

#define DELAYNUM 48

START:
LEDON:  
	SET	r30.t5
	MOV	r1, DELAYNUM

DELAY:  
	SUB	r1, r1, 1         // decrement loop counter
	QBNE	DELAY, r1, 0      // repeat loop unless zero

LEDOFF: 
	CLR	r30.t5
	MOV	r1, DELAYNUM

DELAY2:
	SUB	r1, r1, 1         // decrement loop counter
	QBNE	DELAY2, r1, 0     // repeat loop unless zero
        QBA	LEDON		  // quick branch always
