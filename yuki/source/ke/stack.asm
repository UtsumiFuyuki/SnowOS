global setupStack
section .text

setupStack:
    mov rax, rsp
    mov rsp, [rcx+120]

    push [rcx+128]

    push 0x0
    push 0x0
    push 0x0
    push 0x0
    push 0x0
    push 0x0
    push 0x0
    push 0x0
    push 0x0
    push 0x0
    push 0x0
    push 0x0
    push 0x0
    push 0x0
    push 0x0

    mov [rcx+120], rsp
    mov rsp, rax
    ret