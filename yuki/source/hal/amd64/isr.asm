;
;Snow Operating System
;Copyright (c) UtsumiFuyuki 2025
 
;File: hal/isr.asm

;Description:
;This file holds the ISR Stubs for the
;Interrupt Descriptor Table

;Author:
;UtsumiFuyuki
;October 28th 2025
;

extern keInterruptHandler
extern keTimerHandler
extern keIrqHandler

%macro ISR_ERR 1
ISR_STUB_%+%1:
    push %1
    jmp keInterruptDispatch
%endmacro

%macro ISR_NOERR 1
ISR_STUB_%+%1:
    push 0x0
    push %1
    jmp keInterruptDispatch
%endmacro

%macro TIMER 1
TIMER_STUB_%+%1:
    jmp keTimerHandler
    iretq
%endmacro

%macro IRQ 1
IRQ_STUB_%+%1:
    jmp keIrqHandler
    iretq
%endmacro

keInterruptDispatch:
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
    lea rdx, [rsp + 128]
    mov r8, [rsp + 120]

    cld
    sti
    call keInterruptHandler
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

    add rsp, 16
    iretq

ISR_NOERR  0 ; (#DE) Divison Error
ISR_NOERR  1 ; (#DB) 
ISR_NOERR  2 ; (#)
ISR_NOERR  3 ; (#)
ISR_NOERR  4 ; (#)
ISR_NOERR  5 ; (#)
ISR_NOERR  6 ; (#)
ISR_NOERR  7 ; (#)
ISR_ERR    8 ; (#)
ISR_NOERR  9 ; (#)
ISR_ERR    10 ; (#)
ISR_ERR    11 ; (#)
ISR_ERR    12 ; (#)
ISR_ERR    13 ; (#)
ISR_ERR    14 ; (#)
ISR_NOERR  15 ; (#)
ISR_NOERR  16 ; (#)
ISR_ERR    17 ; (#)
ISR_NOERR  18 ; (#)
ISR_NOERR  19 ; (#)
ISR_NOERR  20 ; (#)
ISR_NOERR  21 ; (#)
ISR_NOERR  22 ; (#)
ISR_NOERR  23 ; (#)
ISR_NOERR  24 ; (#)
ISR_NOERR  25 ; (#)
ISR_NOERR  26 ; (#)
ISR_NOERR  27 ; (#)
ISR_NOERR  28 ; (#)
ISR_NOERR  29 ; (#)
ISR_ERR    30 ; (#)
ISR_NOERR  31 ; (#)

TIMER 32
IRQ 33

global isrStubTable

section .text

isrStubTable:
%assign i 0 
%rep    32
    dq ISR_STUB_%+i
%assign i i+1 
%endrep
dq TIMER_STUB_%+i