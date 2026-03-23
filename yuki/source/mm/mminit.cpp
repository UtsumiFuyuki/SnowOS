/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/mminit.cpp

Description:
This file contains some basic initialization subroutines
for the Mm subsystem of Yuki

Author:
UtsumiFuyuki
March 23rd 2026
**/

#include <mm/mm.hpp>
#include <hal/hal.hpp>
#include <hal/paging.hpp>
#include <ke/log.hpp>

PPFN_ENTRY PFNDB{};
UINT_PTR PFNDBVirtualStart{0xFFFFFA8000000000};

// The amount of usable pages in the system
ULONG64 TotalPages{};

// Setup the PFNdb and Freelist allocator
VOID Mm::Initialize()
{
    // Start the PFNdb at 0xFFFFFA8000000000
    UINT_PTR PFNDBStart = Mm::EarlyAllocatePage();
    Hal::MapPage(PFNDBStart, PFNDBVirtualStart, PAGE_WRITE | PAGE_NO_EXECUTE);

    PFNDB = reinterpret_cast<PPFN_ENTRY>(PFNDBVirtualStart);

    // Start iterating through memory map
    limine_memmap_response *MemoryMap = Hal::RetrieveMemoryMap();
    UINT64 NumberOfPages{};

    for (UINT64 i = 0; i < MemoryMap->entry_count; i++)
    {
        UINT64 Start = (PFNDBVirtualStart + (NumberOfPages * sizeof(PFN_ENTRY)));

        // The Virtual Address to which the region extends to
        UINT64 Length = Start + ((MemoryMap->entries[i]->length / 0x1000) * sizeof(PFN_ENTRY));

        if (Hal::VirtualToPhysical(Start & ~0xFFF) == 0 && MemoryMap->entries[i]->type == LIMINE_MEMMAP_USABLE)
        {
            // Virtual Address is not backed
            for (UINT64 k = 0; k < (((Length - Start) & ~0xFFF) / 0x1000) + 1; k++)
            {
                UINT_PTR BackingPage = Mm::EarlyAllocatePage();

                if (BackingPage == 0)
                {
                    Ke::Print("Failed to allocate a backing page!\r\n");
                    return;
                }

                Hal::MapPage(BackingPage, (Start & ~0xFFF) + (k * 0x1000), PAGE_WRITE | PAGE_NO_EXECUTE);
            }
        }

        NumberOfPages += (MemoryMap->entries[i]->length / 0x1000);
    }

    Ke::Print("PFNdb Initialized!\r\n");
}