/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/vmem.hpp

Description:
This file contains function declarations for the Vmem implementation

Author:
UtsumiFuyuki
March 31rd 2026
**/

#pragma once

#include <cstdint>
#include <utils/list.hpp>

#define VMEM_SEGMENT_SPAN 0
#define VMEM_SEGMENT_FREE 1
#define VMEM_SEGMENT_ALLOCATED 2

typedef struct _VMEM_BOUNDARY_TAG
{
    uint8_t type;
    uintptr_t segmentBase;
    size_t segmentSize;
} VMEM_BOUNDARY_TAG, *PVMEM_BOUNDARY_TAG;

typedef struct _VMEM_ARENA
{
    const char *name;
    uintptr_t base;
    size_t size;
    uint64_t quantum;
    // Imports
    uintptr_t (*allocFunction)(_VMEM_ARENA*, size_t);
    void (*freeFunction)(_VMEM_ARENA*, void*, size_t);
    _VMEM_ARENA *source;
    // TODO: Implement Quantum Caching
    LIST<VMEM_BOUNDARY_TAG> segmentList;
    // TODO: Add hash allocation chain
    LIST<VMEM_BOUNDARY_TAG> freelists[64];
} VMEM_ARENA, *PVMEM_ARENA;

namespace mm
{
    int vmemCreateArena(PVMEM_ARENA arena,
                    const char *name,
                    uintptr_t base,
                    size_t size,
                    uint64_t quantum,
                    uintptr_t (*allocFunction)(PVMEM_ARENA, size_t),
                    void (*freeFunction)(_VMEM_ARENA*, void*, size_t),
                    PVMEM_ARENA source);

    uintptr_t vmemAllocate(PVMEM_ARENA arena, size_t size);
    void vmemFree(PVMEM_ARENA arena, uintptr_t address, size_t size);
};