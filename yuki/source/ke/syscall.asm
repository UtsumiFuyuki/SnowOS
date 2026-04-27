extern keSyscallHandler
extern currentThread

global syscallHandler
section .text

syscallHandler:
    swapgs

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

    sti
    call keSyscallHandler
    cli

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

    swapgs

    o64 sysret

global switchToUser
switchToUser:
    pushfq
    pop r11
    mov rsp, rdx
    o64 sysret