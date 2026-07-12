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
#include <ke/panic.hpp>
#include <utils/mmio.hpp>

extern "C" uintptr_t apicMmioBase;
extern "C" void keSchedule();

extern "C" [[noreturn]] void keInterruptHandler(INTERRUPT_REGISTERS* savedRegisters,
                                                            CPU_STACK_FRAME *cpuSavedRegisters,
                                                            uint64_t interruptVector) {
    INTERRUPT_FRAME intFrame;
    intFrame.vector = interruptVector;
    intFrame.cpuRegisters = *cpuSavedRegisters;
    intFrame.registers = *savedRegisters;

    kePanic("Exception occured!", &intFrame);

    hal::haltCpu();
}

extern "C" void keTimerHandler() {
    mmioWrite32(reinterpret_cast<uint64_t *>(apicMmioBase + LAPIC_EOI_REG), 0);
    __asm__ volatile("sti");
    keSchedule();
    return;
}

extern "C" void keIrqHandler() {
    ke::print("IRQ Recieved!\r\n");
    return;
}