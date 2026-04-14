/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: ke/kernel.cpp

Description:
Function definitions and structs for the CPUID wrapper

Author:
UtsumiFuyuki
March 24th 2026
**/

#pragma once

#include <cstdint>

typedef struct _CPUID_REGISTERS {
    uint64_t rax{};
    uint64_t rbx{};
    uint64_t rcx{};
    uint64_t rdx{};
} CPUID_REGISTERS;

namespace hal {
    namespace x64 {
        static inline CPUID_REGISTERS getCpuid(uint64_t rax, uint64_t rcx) {
            CPUID_REGISTERS cpuidRegs{};

            __asm__ volatile (
                "mov %4, %%rax;"
                "mov %5, %%rcx;"
                "cpuid" :
                "=a"(cpuidRegs.rax),
                "=b"(cpuidRegs.rbx),
                "=c"(cpuidRegs.rcx),
                "=d"(cpuidRegs.rdx) :
                "a"(rax),
                "c"(rcx)
            );
            return cpuidRegs;
        }
    }
}