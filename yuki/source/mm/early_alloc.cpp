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

#include "typedefs.hpp"
#include <limine.h>
#include <hal/hal.hpp>
#include <hal/amd64/paging.hpp>
#include <mm/early_alloc.hpp>
#include <ke/log.hpp>

EARLY_ALLOC_REGION LargestRegion{.Base = 0, .Size = 0};

extern "C" UINT64 BootstrapMemoryBase{};
extern "C" UINT64 BootstrapMemoryAllocated{};

// We're currently bump-allocating virtual addresses, it's messy, but should work for now
UINT64 VirtRegion = 0xFFFFFFFFA0000000;

inline UINT64 TotalPagesAllocated{};

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
            LargestRegion.Base = MemoryMap->entries[i]->base;
            LargestRegion.Size = MemoryMap->entries[i]->length / 0x1000;
        }
    }
    BootstrapMemoryBase = LargestRegion.Base;

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

    BootstrapMemoryAllocated++;

    return Address;
}

PVOID Mm::MapPhysicalAddress(UINT64 PhysicalAddress, UINT64 Length)
{
    UINT64 AlignedPhysicalAddress = (PhysicalAddress & ~0xFFF);
    Length += (PhysicalAddress - AlignedPhysicalAddress);

    if ((Length & 0xFFF) != 0)
    {
        // Round up length
        Length = (Length & ~0xFFF) + 0x2000;
    }

    UINT64 VirtualAddress = (VirtRegion + (TotalPagesAllocated * 0x1000));

    Hal::X64::MapPages(AlignedPhysicalAddress, VirtualAddress, Length, PTE_WRITE);
    Ke::Log(__FILE__, "%llu pages mapped, starting at 0x%llX\r\n", (Length / 0x1000), VirtRegion);
    
    TotalPagesAllocated += (Length / 0x1000) - 1;

    return reinterpret_cast<PVOID>(VirtualAddress + (PhysicalAddress - AlignedPhysicalAddress));
}