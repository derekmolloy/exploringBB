        .cdecls "main.c"
        .clink
        .global START
START:
     LDI32   r1, 0x25        ; set r1 = 0x25 = 37 (dec)
     LDI32   r2, 4           ; set r2 = 100 (binary) = 4 (dec)
     ADD     r1, r1, 5       ; set r1 = r1 + 5 = 42 (dec)
     ADD     r2, r2, 1<<4    ; set r2 = r2 + 10000 (bin) = 20 (dec)
     ADD     r1, r2, r1.w0   ; set r1 = r2 + r1.w0 = 20 + 42 = 62 (dec)
     LDI32   r0, 0x00002000  ; place PRU1 data RAM1 base address in r0
     SBBO    &r1, r0, 4, 4   ; write r1 to the address that is stored in r0
                             ; offset = 4 bytes, size of data = 4 bytes
END:                         ; notify the calling app that finished
     HALT                    ; halt the pru program

