/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/early_alloc.hpp

Description:
This file contains functions declarations for the
bootstrap allocator

Author:
UtsumiFuyuki
March 19th 2026
**/

#pragma once

#include <cstdint>

typedef struct _EARLY_ALLOC_REGION {
    uintptr_t base;
    uint64_t size;
} EARLY_ALLOC_REGION;

namespace mm {
    void earlyInit();
    uintptr_t earlyAllocatePage();
    void *mapPhysicalAddress(uint64_t physicalAddress, uint64_t length);
}