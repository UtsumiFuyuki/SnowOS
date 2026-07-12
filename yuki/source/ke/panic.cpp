/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: ke/panic.cpp

Description:
kernel panic

Author:
UtsumiFuyuki
July 10th, 2026
**/

#include <ke/panic.hpp>
#include <hal/hal.hpp>
#include <hal/amd64/interrupts.hpp>
#include <ke/log.hpp>

extern "C" [[noreturn]] void kePanic(const char *reason, INTERRUPT_FRAME *intFrame) {
    ke::print(ANSI_RED "\r\n========== Kernel Panic!!! ==========\r\n");

    if (reason != nullptr)
        ke::print("Reason: %s\r\n", reason);

    if (intFrame != nullptr) {
        ke::print("Exception: ");

        switch(intFrame->vector) {
            case (0x0):
                ke::print("Division Error! (#DE)");
                break;
            case (0x1):
                ke::print("Debug Interrupt! (#DB)");
                break;
            case (0x2):
                ke::print("Non-maskable Interrupt!");
                break;
            case (0x3):
                ke::print("Breakpoint! (#BP)");
                break;
            case (0x4):
                ke::print("Overflow! (#OF)");
                break;
            case (0x5):
                ke::print("Bound Range Exceeded! (#BR)");
                break;
            case (0x6):
                ke::print("Invalid Opcode! (#UD)");
                break;
            case (0x7):
                ke::print("Device not Available! (#UD)");
                break;
            case (0xD):
                ke::print("General Protection Fault! (#GP)");
                break;
            case (0xE):
                ke::print("Page Fault! (#PF)");
                break;
        }

        ke::print(" Error Code: 0x%llX\r\n\r\nRSP: 0x%llX | RFLAGS: 0x%llX\r\nCS: 0x%llX | RIP: 0x%llX\r\n",
        intFrame->cpuRegisters.errorCode,
        intFrame->cpuRegisters.rsp,
        intFrame->cpuRegisters.rflags,
        intFrame->cpuRegisters.cs,
        intFrame->cpuRegisters.rip);

        ke::print("RAX: 0x%llX | RBX: 0x%llX | RCX: 0x%llX | RDX: 0x%llX\r\n",
        intFrame->registers.rax,
        intFrame->registers.rbx,
        intFrame->registers.rcx,
        intFrame->registers.rdx);

        ke::print("RBP: 0x%llX | RDI: 0x%llX | RSI: 0x%llX | R8: 0x%llX\r\n",
        intFrame->registers.rbp,
        intFrame->registers.rdi,
        intFrame->registers.rsi,
        intFrame->registers.r8);

        ke::print("R9: 0x%llX | R10: 0x%llX | R11: 0x%llX | R12: 0x%llX | R13: 0x%llX | R14: 0x%llX | R15: 0x%llX\r\n" ANSI_RESET,
        intFrame->registers.r9,
        intFrame->registers.r10,
        intFrame->registers.r11,
        intFrame->registers.r12,
        intFrame->registers.r13,
        intFrame->registers.r14,
        intFrame->registers.r15);
    }
    else
        ke::log(__FILE__, "No interrupt frame passed to kePanic()\r\n");

    hal::haltCpu();
}