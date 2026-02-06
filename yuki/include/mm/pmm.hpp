/**
Snow Operating System
Copyright (c) BlueSillyDragon 2025, 2026
 
File: mm/pmm.hpp

Description:
This file contains function prototypes
and data structures for the
physical memory manager

Author:
BlueSillyDragon
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

void MmInitializeFreelist(limine_memmap_response *MemoryMap);
uintptr_t MmAllocatePhysicalPage();
void MmFreePhysicalPage(uint64_t Page);