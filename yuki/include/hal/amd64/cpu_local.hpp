/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/amd64/cpu_local.hpp

Description:
Definition of CPU local struct

Author:
UtsumiFuyuki
April 27th 2026
**/

#pragma once

#include <cstdint>
#include <ke/process.hpp>

typedef struct _CPU_LOCAL {
    uint64_t cpuId;
    THREAD *currentThread;
} CPU_LOCAL;

namespace hal {
    namespace x64 {
        void setCpuLocal(CPU_LOCAL *cpuLocal);
    }
}