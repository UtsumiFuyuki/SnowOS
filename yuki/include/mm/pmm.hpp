/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/pmm.hpp

Description:
This file contains function prototypes
and data structures for the
physical memory manager

Author:
UtsumiFuyuki
February 5th 2026 
**/

#pragma once

#include <cstdint>
#include <limine.h>

typedef struct FREELIST_NODE
{
    uint64_t Start;
    uint64_t Length;
    FREELIST_NODE *Next;
} __attribute__((packed)) FREELIST_NODE;

namespace Mm
{
void InitializeFreelist(limine_memmap_response *MemoryMap);
uintptr_t AllocatePhysicalPage();
void FreePhysicalPage(uint64_t Page);
}