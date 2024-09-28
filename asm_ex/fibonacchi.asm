.code

fnc _start:
    rst
    mov a, 0
    mov b, 1
    add a, c, a
    mov c, 5
    jmp _fib

fnc _restart:
    hlt

fnc _fib:
    mov l, 0
    wrt a
    add a, b, a
    mov b, 0
    add b, c, b
    jmp _restart
    mov b, 1
    mov d, 1
    not b, b
    add b, d, b
    add b, c, b
    mov l, 1
    wrt b
    ldm c
    add b, c, b
    mov l, 13
    ldm b
    jmp _restart
    end
