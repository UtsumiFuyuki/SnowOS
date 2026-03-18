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

#include <typedefs.hpp>
#include <hal/hal.hpp>
#include <ke/print.hpp>

typedef struct
{
    UINT64 Rax;
    UINT64 Rbx;
    UINT64 Rcx;
    UINT64 Rdx;
    UINT64 Rbp;
    UINT64 Rdi;
    UINT64 Rsi;
    UINT64 R8;
    UINT64 R9;
    UINT64 R10;
    UINT64 R11;
    UINT64 R12;
    UINT64 R13;
    UINT64 R14;
    UINT64 R15;
    UINT64 InterruptVector;
    UINT64 ErrorCode;
    UINT64 Rip;
    UINT64 Cs;
    UINT64 Rflags;
    UINT64 Rsp;
} __attribute__((packed)) InterruptFrame;

__attribute__((noreturn)) extern "C" VOID KeInterruptHandler(InterruptFrame* StackFrame)
{
    Ke::Print(LOG_TYPE::None, "\n" ANSI_BRIGHT_RED "Kernel Panic!!!\n");
    Ke::Print(LOG_TYPE::None, "Stack Frame at: 0x%llX\n\n", StackFrame);

    Ke::Print(LOG_TYPE::None, "Exception: ");

    switch(StackFrame->InterruptVector)
    {
        case (0x0):
            Ke::Print(LOG_TYPE::None, "Division Error!");
            break;
        case (0x1):
            Ke::Print(LOG_TYPE::None, "Debug Interrupt!");
            break;
        case (0x2):
            Ke::Print(LOG_TYPE::None, "Non-maskable Interrupt!");
            break;
        case (0x3):
            Ke::Print(LOG_TYPE::None, "Breakpoint!");
            break;
        case (0x4):
            Ke::Print(LOG_TYPE::None, "Overflow!");
            break;
        case (0x5):
            Ke::Print(LOG_TYPE::None, "Bound Range Exceeded!");
            break;
        case (0x6):
            Ke::Print(LOG_TYPE::None, "Invalid Opcode!");
            break;
        case (0x7):
            Ke::Print(LOG_TYPE::None, "Device not Available!");
            break;
        case (0xD):
            Ke::Print(LOG_TYPE::None, "General Protection Fault!");
            break;
        case (0xE):
            Ke::Print(LOG_TYPE::None, "Page Fault!");
            break;
    }

    Ke::Print(LOG_TYPE::None, " Error Code: 0x%llX\n\nRSP: 0x%llX | RFLAGS: 0x%llX\nCS: 0x%llX | RIP: 0x%llX\n",
    StackFrame->ErrorCode,
    StackFrame->Rsp,
    StackFrame->Rflags,
    StackFrame->Cs,
    StackFrame->Rip);

    Ke::Print(LOG_TYPE::None, "RAX: 0x%llX | RBX: 0x%llX | RCX: 0x%llX | RDX: 0x%llX\n",
    StackFrame->Rax,
    StackFrame->Rbx,
    StackFrame->Rcx,
    StackFrame->Rdx);

    Ke::Print(LOG_TYPE::None, "RBP: 0x%llX | RDI: 0x%llX | RSI: 0x%llX | R8: 0x%llX\n",
    StackFrame->Rbp,
    StackFrame->Rdi,
    StackFrame->Rsi,
    StackFrame->R8);

    Ke::Print(LOG_TYPE::None, "R9: 0x%llX | R10: 0x%llX | R11: 0x%llX | R12: 0x%llX | R13: 0x%llX | R14: 0x%llX | R15: 0x%llX\n",
    StackFrame->R9,
    StackFrame->R10,
    StackFrame->R11,
    StackFrame->R12,
    StackFrame->R13,
    StackFrame->R14,
    StackFrame->R15);
    
    for(;;)
    {
        Hal::HaltCpu();
    }
}