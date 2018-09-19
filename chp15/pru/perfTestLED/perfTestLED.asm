        .cdecls "main.c"
        .clink
        .global START
        .asg "48",  DELAY

START:
        SET     r30, r30.t5
        LDI32   r0, DELAY
DELAYON:
        SUB     r0, r0, 1
        QBNE    DELAYON, r0, 0   ; Loop to DELAYON, unless REG0=0
LEDOFF:
        CLR     r30, r30.t5      ; clear the output bin (LED off)
        LDI32   r0, DELAY        ; Reset REG0 to the length of the delay

DELAYOFF:
        SUB     r0, r0, 1        ; decrement REG0 by 1
        QBNE    DELAYOFF, r0, 0  ; Loop to DELAYOFF, unless REG0=0
        QBBC    START, r31, 3    ; is the button pressed? If not, loop
END:                             ; notify the calling app that finished
        HALT                     ; halt the pru program

