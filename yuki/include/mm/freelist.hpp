/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/freelist.hpp

Description:
This file contains function prototypes and data structures for the
physical memory manager

Author:
UtsumiFuyuki
February 5th 2026 
**/

#pragma once

#include <typedefs.hpp>
#include <limine.h>

typedef struct FREELIST_NODE
{
    UINT64 Start;
    UINT64 Length;
    FREELIST_NODE *Next;
} __attribute__((packed)) FREELIST_NODE;

namespace Mm
{
VOID InitializeFreelist(limine_memmap_response *MemoryMap);
UINT_PTR AllocatePhysicalPage();
VOID FreePhysicalPage(UINT_PTR Page);
}