/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/amd64/msr.hpp

Description:
MSR Helper Functions

Author:
UtsumiFuyuki
April 15th 2026
**/

// APIC MSRs
#define IA32_APIC_BASE      0x0000001B

// FRED MSRs
#define IA32_FRED_CONFIG    0x000001D4
#define IA32_FRED_RSP0      0x000001CC
#define IA32_FRED_RSP1      0x000001CD
#define IA32_FRED_RSP2      0x000001CE
#define IA32_FRED_RSP3      0x000001CF
#define IA32_FRED_STKLVLS   0x000001D0

// SYSCALL Related MSRs
#define IA32_STAR 0xC0000081
#define IA32_LSTAR 0xC0000082

#define IA32_KERNEL_GS_BASE 0xC0000102
#define IA32_EFER           0xC0000080

#pragma once

#include <cstdint>

static inline uint64_t rdmsr(uint64_t msr) {
    uint32_t low{};
    uint32_t high{};

    __asm__ volatile ("rdmsr" : "=a"(low), "=d"(high) : "c"(msr) : "memory");
    return low + (static_cast<uint64_t>(high) << 32);
}

static inline void wrmsr(uint64_t msr, uint64_t value) {
    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = (value >> 32);

    __asm__ volatile ("wrmsr" :: "a"(low), "d"(high), "c"(msr) : "memory");
}