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

%macro isrErrorStub 1
ISR_STUB_%+%1:
    push %1
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
    call keInterruptHandler

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

    iretq
%endmacro

%macro isrNoErrorStub 1
ISR_STUB_%+%1:
    push 0x0
    push %1
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
    call keInterruptHandler

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

    iretq
%endmacro

extern keInterruptHandler
isrNoErrorStub  0
isrNoErrorStub  1
isrNoErrorStub  2
isrNoErrorStub  3
isrNoErrorStub  4
isrNoErrorStub  5
isrNoErrorStub  6
isrNoErrorStub  7
isrErrorStub    8
isrNoErrorStub  9
isrErrorStub    10
isrErrorStub    11
isrErrorStub    12
isrErrorStub    13
isrErrorStub    14
isrNoErrorStub  15
isrNoErrorStub  16
isrErrorStub    17
isrNoErrorStub  18
isrNoErrorStub  19
isrNoErrorStub  20
isrNoErrorStub  21
isrNoErrorStub  22
isrNoErrorStub  23
isrNoErrorStub  24
isrNoErrorStub  25
isrNoErrorStub  26
isrNoErrorStub  27
isrNoErrorStub  28
isrNoErrorStub  29
isrErrorStub    30
isrNoErrorStub  31

global isrStubTable

section .text

isrStubTable:
%assign i 0 
%rep    32
    dq ISR_STUB_%+i
%assign i i+1 
%endrep