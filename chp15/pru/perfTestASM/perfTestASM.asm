        .cdecls "perfTestContainer.c"

        .clink
        .global start
start:
        CLR     r30, r30.t5      ; turn off the LED
        WBS     r31, 3           ; wait bit set - i.e., button press
        SET     r30, r30.t5      ; set the output bit - turn on the LED

        HALT

