/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: ke/interrupts.cpp

Description:
This file contains the interrupt handling
routines for Yuki

Author:
UtsumiFuyuki
October 29th 2025
**/

#include <cstddef>
#include <cstdint>
#include <hal/hal.hpp>
#include <hal/amd64/interrupts.hpp>
#include <hal/amd64/apic/apic.hpp>
#include <ke/log.hpp>
#include <utils/mmio.hpp>

extern "C" uintptr_t apicMmioBase;

extern "C" [[noreturn]] void keInterruptHandler(INTERRUPT_REGISTERS* savedRegisters,
                                                            CPU_STACK_FRAME *cpuSavedRegisters,
                                                            uint64_t interruptVector) {
    ke::print("\n" ANSI_BRIGHT_RED "Kernel Panic!!!\r\n");
    ke::print("Stack Frame at: 0x%llX\r\n\r\n", savedRegisters);

    ke::print("Exception: ");

    switch(interruptVector) {
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
    cpuSavedRegisters->errorCode,
    cpuSavedRegisters->rsp,
    cpuSavedRegisters->rflags,
    cpuSavedRegisters->cs,
    cpuSavedRegisters->rip);

    ke::print("RAX: 0x%llX | RBX: 0x%llX | RCX: 0x%llX | RDX: 0x%llX\r\n",
    savedRegisters->rax,
    savedRegisters->rbx,
    savedRegisters->rcx,
    savedRegisters->rdx);

    ke::print("RBP: 0x%llX | RDI: 0x%llX | RSI: 0x%llX | R8: 0x%llX\r\n",
    savedRegisters->rbp,
    savedRegisters->rdi,
    savedRegisters->rsi,
    savedRegisters->r8);

    ke::print("R9: 0x%llX | R10: 0x%llX | R11: 0x%llX | R12: 0x%llX | R13: 0x%llX | R14: 0x%llX | R15: 0x%llX\r\n",
    savedRegisters->r9,
    savedRegisters->r10,
    savedRegisters->r11,
    savedRegisters->r12,
    savedRegisters->r13,
    savedRegisters->r14,
    savedRegisters->r15);
    
    hal::haltCpu();
}

extern "C" void keTimerHandler() {
    mmioWrite32(reinterpret_cast<uint64_t *>(apicMmioBase + LAPIC_EOI_REG), 0);
    __asm__ volatile("sti");
    ke::print("tick\r\n");
    return;
}

extern "C" void keIrqHandler() {
    ke::print("IRQ Recieved!\r\n");
    return;
}