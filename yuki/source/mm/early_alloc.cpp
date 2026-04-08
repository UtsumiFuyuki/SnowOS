/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/early_alloc.cpp

Description:
This file contains the allocation code for the
bootstrap allocator

Author:
UtsumiFuyuki
March 19th 2026
**/

#include <limine.h>
#include <hal/hal.hpp>
#include <hal/amd64/paging.hpp>
#include <mm/early_alloc.hpp>
#include <ke/log.hpp>

EARLY_ALLOC_REGION largestRegion{.base = 0, .size = 0};

extern "C" uint64_t bootstrapMemoryBase{};
extern "C" uint64_t bootstrapMemoryAllocated{};

inline uint64_t TotalPagesAllocated{};

// Simple bump allocator for bootstrapping
// TODO: there may not be a memmap entry big enough to map everything, iterate through memmap instead
void mm::earlyInit() {
    ke::log(__FILE__, "Initializing bump allocator...\r\n");

    limine_memmap_response *memoryMap = hal::retrieveMemoryMap();

    for (uint64_t i = 0; i < memoryMap->entry_count; i++) {
        if ((memoryMap->entries[i]->length / 0x1000) > largestRegion.size && memoryMap->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            largestRegion.base = memoryMap->entries[i]->base;
            largestRegion.size = memoryMap->entries[i]->length / 0x1000;
        }
    }
    bootstrapMemoryBase = largestRegion.base;

    ke::print("Initialized bootstrap allocator!\r\n");
}

uintptr_t mm::earlyAllocatePage() {
    uintptr_t address = largestRegion.base;

    if (largestRegion.size == 0) {
        ke::log(__FILE__, "Attempt to allocate a page failed due to OOM!\r\n");
        return 0;
    }

    largestRegion.base += 0x1000;
    largestRegion.size--;

    bootstrapMemoryAllocated++;

    return address;
}