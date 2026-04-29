extern keSyscallHandler
extern currentThread

global syscallHandler
section .text

syscallHandler:
    cli
    swapgs

    mov [gs:0x10], r15
    mov r15, [gs:0x08]
    mov [r15 + 120], rsp
    mov rsp, [r15 + 152]
    mov r15, [gs:0x10]

    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rsi
    push rdi
    push rbp
    push rdx
    push rcx
    push rbx
    push rax

    mov rcx, rsp

    call keSyscallHandler

    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rbp
    pop rdi
    pop rsi
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15

    mov [gs:0x10], r15
    mov r15, [gs:0x08]
    mov [r15 + 152], rsp
    mov rsp, [r15 + 120]
    mov r15, [gs:0x10]

    swapgs

    o64 sysret

global switchToUser
switchToUser:
    pushfq
    pop r11
    mov rsp, rdx
    o64 sysret