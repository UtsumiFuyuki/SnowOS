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

UINT64 BumpStart{};

// Simple bump allocator for bootstrapping
VOID Mm::EarlyInit()
{
    Ke::Log(__FILE__, "Initializing bump allocator...\r\n");

    limine_memmap_response *MemoryMap = Hal::RetrieveMemoryMap();
    UINT64 LargestArea{};

    for (UINT64 i = 0; i < MemoryMap->entry_count; i++)
    {
        if ((MemoryMap->entries[i]->length / 0x1000) > LargestArea && MemoryMap->entries[i]->type == LIMINE_MEMMAP_USABLE)
        {
            Ke::Log(__FILE__, "New largest area found at 0x%llX\r\n", MemoryMap->entries[i]->base);
            BumpStart = MemoryMap->entries[i]->base + Hal::RetrieveHhdmOffset();
            LargestArea = (MemoryMap->entries[i]->length / 0x1000);
        }
    }
    Ke::Print("Initialized bootstrap allocator!\r\n");
}

UINT64 *Mm::EarlyAllocatePage()
{
    UINT64 *Address = reinterpret_cast<UINT64 *>(BumpStart);
    BumpStart += 0x1000;
    return Address;
}