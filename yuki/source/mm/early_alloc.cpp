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
#include <mm/early_alloc.hpp>
#include <ke/log.hpp>

EARLY_ALLOC_REGION LargestRegion{.Base = 0, .Size = 0};

// Simple bump allocator for bootstrapping
// TODO: there may not be a memmap entry big enough to map everything, iterate through memmap instead
VOID Mm::EarlyInit()
{
    Ke::Log(__FILE__, "Initializing bump allocator...\r\n");

    limine_memmap_response *MemoryMap = Hal::RetrieveMemoryMap();

    for (UINT64 i = 0; i < MemoryMap->entry_count; i++)
    {
        if ((MemoryMap->entries[i]->length / 0x1000) > LargestRegion.Size && MemoryMap->entries[i]->type == LIMINE_MEMMAP_USABLE)
        {
            Ke::Log(__FILE__, "New largest area found at 0x%llX\r\n", MemoryMap->entries[i]->base);
            LargestRegion.Base = MemoryMap->entries[i]->base;
            LargestRegion.Size = MemoryMap->entries[i]->length / 0x1000;
        }
    }
    Ke::Print("Initialized bootstrap allocator!\r\n");
}

UINT_PTR Mm::EarlyAllocatePage()
{
    UINT_PTR Address = LargestRegion.Base;

    if (LargestRegion.Size == 0)
    {
        Ke::Log(__FILE__, "Attempt to allocate a page failed due to OOM!\r\n");
        return 0;
    }

    LargestRegion.Base += 0x1000;
    LargestRegion.Size--;

    return Address;
}