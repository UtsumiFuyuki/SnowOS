global fredRing3Stub
global fredRing0Stub
global fredRing3Entry
global fredRing0Entry

extern halFredRing3Handler
extern halFredRing0Handler

section .text
align 4096 ; FRED Interrupt handlers need to be page-aligned

; Entry Stub for Ring 3
fredRing3Stub:
    jmp fredRing3Entry

times 256 - ($ - fredRing3Stub) db 0 ; Ring 0 Entry Stub should be located at (IA32_FRED_CONFIG & 0xFFF) + 256

; Entry Stub for Ring 0
fredRing0Stub:
    jmp fredRing0Entry

fredRing3Entry:
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

    sti
    call halFredRing3Handler
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

    eretu ; Event Return to User

fredRing0Entry:
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

    sti
    call halFredRing0Handler
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

    erets ; Event Return to Supervisor