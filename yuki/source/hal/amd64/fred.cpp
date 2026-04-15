/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/amd64/fred.cpp

Description:
Implementation for FRED

Author:
UtsumiFuyuki
April 15th 2026
**/

#include "hal/amd64/interrupts.hpp"
#include <hal/amd64/fred.hpp>
#include <hal/amd64/msr.hpp>
#include <ke/log.hpp>

bool fredEnabled{false};

extern "C" void fredRing3Stub();

[[gnu::aligned(64)]] uint8_t dfStack[4096];
[[gnu::aligned(64)]] uint8_t nmiStack[4096];

void hal::x64::initializeFred() {
    ke::log(__FILE__, "Initializing FRED...\r\n");
    __asm__ volatile (
                    "mov %cr4, %rax;"
                    "bts $32, %rax;"
                    "mov %rax, %cr4");
    
    uint64_t star = (static_cast<uint64_t>(0x18) | 3) | (static_cast<uint64_t>(0x08) << 32);
    wrmsr(IA32_STAR, star);
    wrmsr(IA32_FRED_CONFIG, reinterpret_cast<uint64_t>(fredRing3Stub));
    wrmsr(IA32_FRED_RSP2, reinterpret_cast<uint64_t>(&dfStack) + sizeof(dfStack));
    wrmsr(IA32_FRED_RSP3, reinterpret_cast<uint64_t>(&nmiStack) + sizeof(nmiStack));
    wrmsr(IA32_FRED_STKLVLS, FRED_STKLVL(0x8, 2) | FRED_STKLVL(0x2, 3));

    ke::log(__FILE__, "FRED Initialized!\r\n");
    fredEnabled = true;
}

extern "C" void halFredRing3Handler(FRED_STACK_FRAME *stackFrame) {
    ke::print("You did something wrong :p we can't recieve interrupts from ring3 right now!\r\n");
}

extern "C" void halFredRing0Handler(FRED_STACK_FRAME *stackFrame) {
    uint8_t vector = (stackFrame->cpuRegisters.ss >> 32) & 0xFF;
    ke::print("Recieved interrupt 0x%llX from FRED!\r\n", vector);
    keInterruptHandler(&stackFrame->registers, &stackFrame->cpuRegisters, vector);
}