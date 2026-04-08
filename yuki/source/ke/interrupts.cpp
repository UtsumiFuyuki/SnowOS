/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025
 
File: ke/interrupts.cpp

Description:
This file contains the interrupt handling
routines for Yuki

Author:
UtsumiFuyuki
October 29th 2025
**/

#include <cstdint>
#include <hal/hal.hpp>
#include <ke/log.hpp>

typedef struct _INTERRUPT_FRAME {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t interruptVector;
    uint64_t errorCode;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
} __attribute__((packed)) INTERRUPT_FRAME;

__attribute__((noreturn)) extern "C" void keInterruptHandler(INTERRUPT_FRAME* stackFrame) {
    ke::print("\n" ANSI_BRIGHT_RED "Kernel Panic!!!\r\n");
    ke::print("Stack Frame at: 0x%llX\r\n\r\n", stackFrame);

    ke::print("Exception: ");

    switch(stackFrame->interruptVector) {
        case (0x0):
            ke::print("Division Error!");
            break;
        case (0x1):
            ke::print("Debug Interrupt!");
            break;
        case (0x2):
            ke::print("Non-maskable Interrupt!");
            break;
        case (0x3):
            ke::print("Breakpoint!");
            break;
        case (0x4):
            ke::print("Overflow!");
            break;
        case (0x5):
            ke::print("Bound Range Exceeded!");
            break;
        case (0x6):
            ke::print("Invalid Opcode!");
            break;
        case (0x7):
            ke::print("Device not Available!");
            break;
        case (0xD):
            ke::print("General Protection Fault! (#GP)");
            break;
        case (0xE):
            ke::print("Page Fault! (#PF)");
            break;
    }

    ke::print(" Error Code: 0x%llX\r\n\r\nRSP: 0x%llX | RFLAGS: 0x%llX\r\nCS: 0x%llX | RIP: 0x%llX\r\n",
    stackFrame->errorCode,
    stackFrame->rsp,
    stackFrame->rflags,
    stackFrame->cs,
    stackFrame->rip);

    ke::print("RAX: 0x%llX | RBX: 0x%llX | RCX: 0x%llX | RDX: 0x%llX\r\n",
    stackFrame->rax,
    stackFrame->rbx,
    stackFrame->rcx,
    stackFrame->rdx);

    ke::print("RBP: 0x%llX | RDI: 0x%llX | RSI: 0x%llX | R8: 0x%llX\r\n",
    stackFrame->rbp,
    stackFrame->rdi,
    stackFrame->rsi,
    stackFrame->r8);

    ke::print("R9: 0x%llX | R10: 0x%llX | R11: 0x%llX | R12: 0x%llX | R13: 0x%llX | R14: 0x%llX | R15: 0x%llX\r\n",
    stackFrame->r9,
    stackFrame->r10,
    stackFrame->r11,
    stackFrame->r12,
    stackFrame->r13,
    stackFrame->r14,
    stackFrame->r15);
    
    hal::haltCpu();
}