/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/mm.hpp

Description:
This file contains functions declarations for the
physical memory allocator

Author:
UtsumiFuyuki
March 23rd 2026
**/

#pragma once

#include <mm/early_alloc.hpp>
#include <utils/list.hpp>

typedef uint64_t PFN_NUMBER;

typedef struct _PFN_ENTRY
{
    uint64_t free;

    // Points to another PFNdb entry, used to build the Free List
    _PFN_ENTRY *pageEntry;
} PFN_ENTRY, *PPFN_ENTRY;

// A list of PFNdb entries
typedef struct _PAGE_LIST
{
    uint64_t pageCount;
    PPFN_ENTRY head;
} PAGE_LIST, *PPAGE_LIST;

namespace mm
{
    void initialize();
    uintptr_t allocatePage();
    void freePage(uintptr_t physicalAddress);
    void initializeVmm();

    void *allocateKernelPages(size_t pages);
    void freeKernelPages(uintptr_t address);
    uintptr_t allocateKernelVirt(size_t pages);
    void freeKernelVirt(uintptr_t address, size_t pages);
}