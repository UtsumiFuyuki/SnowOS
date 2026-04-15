/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/amd64/fred.hpp

Description:
Structures and function declarations for FRED

Author:
UtsumiFuyuki
April 15th 2026
**/

#pragma once

#include <cstdint>
#include <hal/amd64/interrupts.hpp>

#define FRED_STKLVL(n, lvl)  ((uint64_t)(lvl) << ((n) * 2))

typedef struct _FRED_STACK_FRAME {
    INTERRUPT_REGISTERS registers;
    CPU_STACK_FRAME cpuRegisters;
    uint64_t eventData;
    uint64_t reserved;
} FRED_STACK_FRAME;

extern "C" bool fredEnabled;

namespace hal {
    namespace x64 {
        void initializeFred();
    };
};