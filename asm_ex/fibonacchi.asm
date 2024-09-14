.code

fnc _start:
    rst
    mov a, 1
    mov b, 1
    add a, c, a
    mov l, 0x0d
    mov c, 5
    jmp _fib
    hlt

fnc _fib:
    mov l, 0
    wrt a
    add a, b, a
    mov b, 0
    add b, c, b
    mov l, 0x0b
    jmp _start
    mov b, 1
    mov d, 1
    not b, b
    add b, d, b
    add b, c, b
    mov l, 1
    wrt b
    lda c
    add b, c, b
    mov l, 0x0d
    lda b
    jmp _start