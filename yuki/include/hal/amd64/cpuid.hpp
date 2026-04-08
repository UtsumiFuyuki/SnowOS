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

typedef struct _CPUID {
    uint64_t rax{};
    uint64_t rbx{};
    uint64_t rcx{};
    uint64_t rdx{};
} CPUID;

namespace hal {
    namespace x64 {
        CPUID getCpuid(uint64_t rax);
    }
}