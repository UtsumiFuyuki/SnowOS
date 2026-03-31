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

#include <typedefs.hpp>
#include <mm/early_alloc.hpp>
#include <utils/list.hpp>

#define VMEM_SEGMENT_FREE 0x1
#define VMEM_SEGMENT_SPAN 0x2

typedef ULONG64 PFN_NUMBER;

typedef struct _PFN_ENTRY
{
    UINT64 Free;

    // Points to another PFNdb entry, used to build the Free List
    _PFN_ENTRY *PageEntry;
} PFN_ENTRY, *PPFN_ENTRY;

// A list of PFNdb entries
typedef struct _PAGE_LIST
{
    UINT64 PageCount;
    PPFN_ENTRY Head;
} PAGE_LIST, *PPAGE_LIST;

namespace Mm
{
    VOID Initialize();
    UINT_PTR AllocatePage();
    VOID FreePage(UINT_PTR PhysicalAddress);
    VOID InitializeVmm();
}