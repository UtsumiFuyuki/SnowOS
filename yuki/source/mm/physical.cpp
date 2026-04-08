/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/physical.cpp

Description:
This file contains the physical page allocator for Yuki

Author:
UtsumiFuyuki
March 23rd 2026
**/

#include <mm/mm.hpp>
#include <hal/hal.hpp>
#include <hal/paging.hpp>
#include <ke/log.hpp>

PPFN_ENTRY pfndb{};
uintptr_t pfndbVirtualStart{0xFFFFFA8000000000};

// The amount of usable pages in the system
uint64_t totalPages{};

// List of free pages in the PFNdb
PAGE_LIST freelist{.pageCount = 0, .head = nullptr};

extern "C" uint64_t bootstrapMemoryBase;
extern "C" uint64_t bootstrapMemoryAllocated;

// Setup the PFNdb and Freelist allocator
void mm::initialize() {
    // Start the PFNdb at 0xFFFFFA8000000000
    uintptr_t pfndbStart = mm::earlyAllocatePage();
    hal::mapPage(pfndbStart, pfndbVirtualStart, PAGE_WRITE | PAGE_NO_EXECUTE);

    pfndb = reinterpret_cast<PPFN_ENTRY>(pfndbVirtualStart);

    // Start iterating through memory map
    limine_memmap_response *memoryMap = hal::retrieveMemoryMap();

    for (size_t i = 0; i < memoryMap->entry_count; i++) {
        uint64_t start = (pfndbVirtualStart + ((memoryMap->entries[i]->base >> 12) * sizeof(PFN_ENTRY)));

        // The Virtual Address to which the region extends to
        uint64_t length =start + (((memoryMap->entries[i]->base + memoryMap->entries[i]->length) >> 12) * sizeof(PFN_ENTRY));

        if (hal::virtualToPhysical((length & ~0xFFF) - 0x1000) == 0 && memoryMap->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            uint64_t PagesToAlloc = (((length & ~0xFFF) - (start & ~0xFFF)) / 0x1000);

            // Virtual Address is not backed
            for (uint64_t k = 0; k <= PagesToAlloc + 1; k++) {
                uintptr_t backingPage = mm::earlyAllocatePage();

                if (backingPage == 0) {
                    ke::print("Failed to allocate a backing page!\r\n");
                    return;
                }

                hal::mapPage(backingPage, (start & ~0xFFF) + (k * 0x1000), PAGE_WRITE | PAGE_NO_EXECUTE);
            }
        }
    }

    // Initialize all entries in the PFNdb
    for (uint64_t i = 0; i < memoryMap->entry_count; i++) {
        if (memoryMap->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            for (size_t k = 0; k < memoryMap->entries[i]->length; k += 0x1000) {
                uintptr_t physicalBase = (memoryMap->entries[i]->base + k);
                PFN_NUMBER pfn = (physicalBase >> 12);

                // Don't mark bootstrap memory as free
                if (physicalBase >= bootstrapMemoryBase && physicalBase < (bootstrapMemoryBase + (bootstrapMemoryAllocated * 0x1000)))
                    pfndb[pfn] = {.free = 0, .pageEntry = nullptr};

                else {
                    pfndb[pfn] = {.free = 1, .pageEntry = freelist.head};
                    freelist.head = &pfndb[pfn];
                    freelist.pageCount++;
                }
            }

            totalPages += memoryMap->entries[i]->length / 0x1000;
        }
    }

    ke::print("PFNdb Initialized!\r\n");
}

// Returns the physical address of a free page
uintptr_t mm::allocatePage()
{
    if (freelist.head == nullptr) {
        // TODO: panic on OOM
        ke::log(__FILE__, "Couldn't allocate a page due to OOM!\r\n");
        return 0;
    }
    uintptr_t address = reinterpret_cast<uintptr_t>(freelist.head) - pfndbVirtualStart;

    freelist.head = freelist.head->pageEntry;
    freelist.pageCount--;

    return reinterpret_cast<uintptr_t>((address / sizeof(PFN_ENTRY)) << 12);
}

void mm::freePage(uintptr_t physicalAddress) {
    PFN_NUMBER pfn = (physicalAddress >> 12);

    pfndb[pfn].free = 1;
    pfndb[pfn].pageEntry = freelist.head;
    freelist.head = &pfndb[pfn];
    freelist.pageCount++;
}