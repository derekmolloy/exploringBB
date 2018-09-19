        .cdecls "pruTest.c"
        .clink
        .global start
start:
        LDI32   r0, 0x00000000   ; load r0 with the address to load
        LBBO    &r1, r0, 0, 4    ; load r1 with r0 the EBBFEED0
        LDI32   r0, 0x00000004   ; load r0 with the next address
        LBBO    &r2, r0, 0, 4    ; load r2 with the EBBFEED1

        LDI32   r0, 0x00002000   ; load r0 with the PRU1 address to load
        LBBO    &r3, r0, 0, 4    ; load r3 with r0 the EBBFEED2
        LDI32   r0, 0x00002004   ; load r0 with the next PRU1 address
        LBBO    &r4, r0, 0, 4    ; load r4 with the EBBFEED3

        LDI32   r0, 0x00010000   ; load r0 with the shared address
        LBBO    &r5, r0, 0, 4    ; load r5 with the EBBFEED4
        LDI32   r0, 0x00010004   ; load r0 with the shared address
        LBBO    &r6, r0, 0, 4    ; load r6 with the EBBFEED5
        LDI32   r0, 0x00010008   ; load r0 with the shared address
        LBBO    &r7, r0, 0, 4    ; load r6 with the EBBFEED6

        HALT

