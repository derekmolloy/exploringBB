; Written for Exploring BeagleBone v2 by Derek Molloy
; This program flashes an LED on P9_11/P2.05 until a button that is
; attached to P9_13/P2.07 is pressed.
  .cdecls "main.c"
  .clink
  .global START
  .asg  "5000000",  DELAY
  .asg  0x44e07000, GPIO0       ; GPIO Bank 0, See the AM335x TRM
  .asg  0x4804c000, GPIO1       ; GPIO Bank 1, Table 2.2 Peripheral Map
  .asg  0x481ac000, GPIO2       ; GPIO Bank 2,
  .asg  0x481ae000, GPIO3       ; GPIO Bank 3,
  .asg  0x190, GPIO_CLRDATAOUT  ; for clearing the GPIO registers
  .asg  0x194, GPIO_SETDATAOUT  ; for setting the GPIO registers
  .asg  0x138, GPIO_DATAOUT     ; for reading the GPIO registers
  .asg  1<<30, GPIO0_30         ; P9_11 gpio0[30] Output - bit 30
  .asg  1<<31, GPIO0_31         ; P9_13 gpio0[31] Input - bit 31
  .asg  32, PRU0_R31_VEC_VALID  ; allows notification of program completion
  .asg  3,  PRU_EVTOUT_0        ; the event number that is sent back

START:
        ; Enable the OCP master port
        LBCO    &r0, C4, 4, 4    ; load SYSCFG reg into r0 (use c4 const addr)
        CLR     r0, r0, 4        ; clear bit 4 (STANDBY_INIT)
        SBCO    &r0, C4, 4, 4    ; store the modified r0 back at the load addr

MAINLOOP:
        LDI32   r1, (GPIO0|GPIO_SETDATAOUT)  ; load addr for GPIO Set data r1
        LDI32   r2, GPIO0_30     ; write GPIO0_30 to r2
        SBBO    &r2, r1, 0, 4     ; write r2 to the r1 address value - LED ON
        LDI32   r0, DELAY        ; store the length of the delay in REG0
DELAYON:
        SUB     r0, r0, 1
        QBNE    DELAYON, r0, 0   ; Loop to DELAYON, unless REG0=0
LEDOFF:
        LDI32   r1, (GPIO0|GPIO_CLRDATAOUT) ; load addr for GPIO Clear data
        LDI32   r2, GPIO0_30     ; write GPIO_30 to r2
        SBBO    &r2, r1, 0, 4    ; write r2 to the r1 address - LED OFF
        LDI32   r0, DELAY        ; Reset REG0 to the length of the delay
DELAYOFF:
        SUB     r0, r0, 1        ; decrement REG0 by 1
        QBNE    DELAYOFF, r0, 0  ; Loop to DELAYOFF, unless REG0=0

        LDI32   r5, (GPIO0|GPIO_DATAOUT)    ; load read addr for DATAOUT
        LBBO    &r6, r5, 0, 4    ; Load the value at r5 into r6
        QBBC    MAINLOOP, r6, 31 ; is the button pressed? If not, loop
END:                             ; notify the calling app that finished
        LDI32   R31, (PRU0_R31_VEC_VALID|PRU_EVTOUT_0)
        HALT                     ; halt the pru program
