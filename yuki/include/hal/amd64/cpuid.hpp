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

#include <typedefs.hpp>

typedef struct _CPUID
{
    UINT64 Rax{};
    UINT64 Rbx{};
    UINT64 Rcx{};
    UINT64 Rdx{};
} CPUID;

namespace Hal
{
    namespace X64
    {
        CPUID GetCpuid(UINT64 Rax);
    }
}