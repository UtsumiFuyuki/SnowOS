/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/mminit.cpp

Description:
This file contains the physical page allocator
for Yuki

Author:
UtsumiFuyuki
March 23rd 2026
**/

#include "typedefs.hpp"
#include <mm/mm.hpp>
#include <hal/hal.hpp>
#include <hal/paging.hpp>
#include <ke/log.hpp>

PPFN_ENTRY PFNDB{};
UINT_PTR PFNDBVirtualStart{0xFFFFFA8000000000};

// The amount of usable pages in the system
ULONG64 TotalPages{};

// List of free pages in the PFNdb
PAGE_LIST FreeList{.PageCount = 0, .Head = nullptr};

extern "C" UINT64 BootstrapMemoryBase;
extern "C" UINT64 BootstrapMemoryAllocated;

// Setup the PFNdb and Freelist allocator
VOID Mm::Initialize()
{
    // Start the PFNdb at 0xFFFFFA8000000000
    UINT_PTR PFNDBStart = Mm::EarlyAllocatePage();
    Hal::MapPage(PFNDBStart, PFNDBVirtualStart, PAGE_WRITE | PAGE_NO_EXECUTE);

    PFNDB = reinterpret_cast<PPFN_ENTRY>(PFNDBVirtualStart);

    // Start iterating through memory map
    limine_memmap_response *MemoryMap = Hal::RetrieveMemoryMap();

    for (UINT64 i = 0; i < MemoryMap->entry_count; i++)
    {
        UINT64 Start = (PFNDBVirtualStart + ((MemoryMap->entries[i]->base >> 12) * sizeof(PFN_ENTRY)));
        Ke::Log(__FILE__, "Start 0x%llX\r\n", Start);

        // The Virtual Address to which the region extends to
        UINT64 Length = Start + (((MemoryMap->entries[i]->base + MemoryMap->entries[i]->length) >> 12) * sizeof(PFN_ENTRY));
        Ke::Log(__FILE__, "Length 0x%llX\r\n", Length);

        Ke::Log(__FILE__, "Base 0x%llX Length 0x%llX End of Region 0x%llX\r\n",
                    MemoryMap->entries[i]->base,
                    MemoryMap->entries[i]->length,
                    MemoryMap->entries[i]->base + MemoryMap->entries[i]->length);

        if (Hal::VirtualToPhysical((Length & ~0xFFF) - 0x1000) == 0 && MemoryMap->entries[i]->type == LIMINE_MEMMAP_USABLE)
        {
            UINT64 PagesToAlloc = (((Length & ~0xFFF) - (Start & ~0xFFF)) / 0x1000);
            Ke::Log(__FILE__, "Pages to Allocate %llu\r\n\r\n", PagesToAlloc);

            // Virtual Address is not backed
            for (UINT64 k = 0; k <= PagesToAlloc + 1; k++)
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
    }

    Ke::Log(__FILE__, "Bootstrap Memory Base 0x%llX Boostrap Pages Allocated %llu\r\n", BootstrapMemoryBase, BootstrapMemoryAllocated);

    // Initialize all entries in the PFNdb
    for (UINT64 i = 0; i < MemoryMap->entry_count; i++)
    {
        if (MemoryMap->entries[i]->type == LIMINE_MEMMAP_USABLE)
        {
            for (UINT64 k = 0; k < MemoryMap->entries[i]->length; k += 0x1000)
            {
                UINT_PTR PhysicalBase = (MemoryMap->entries[i]->base + k);
                PFN_NUMBER Pfn = ((PhysicalBase & ~0xFFF) >> 12);

                

                // Don't mark bootstrap memory as free
                if (PhysicalBase >= BootstrapMemoryBase && PhysicalBase < (BootstrapMemoryBase + (BootstrapMemoryAllocated * 0x1000)))
                {
                    PFNDB[Pfn] = {.Free = 0, .PageEntry = nullptr};
                }

                else
                {
                    PFNDB[Pfn] = {.Free = 1, .PageEntry = FreeList.Head};
                    FreeList.Head = &PFNDB[Pfn];
                    FreeList.PageCount++;
                }
            }
        }
    }

    Ke::Print("PFNdb Initialized!\r\n");
}

// Returns the physical address of a free page
UINT_PTR Mm::AllocatePage()
{
    if (FreeList.Head == nullptr)
    {
        // TODO: panic on OOM
        Ke::Log(__FILE__, "Couldn't allocate a page due to OOM!\r\n");
        return 0;
    }
    PPFN_ENTRY Entry = FreeList.Head;

    FreeList.Head = FreeList.Head->PageEntry;
    FreeList.PageCount--;
    Ke::Log(__FILE__, "Head is now 0x%llX\r\n", FreeList.Head);

    return reinterpret_cast<UINT_PTR>(Entry);
}