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

#include <typedefs.hpp>
#include <utils/list.hpp>

#define VMEM_SEGMENT_SPAN 0
#define VMEM_SEGMENT_FREE 1
#define VMEM_SEGMENT_ALLOCATED 2

typedef struct _VMEM_BOUNDARY_TAG
{
    UINT8 Type;
    UINT_PTR SegmentStart;
    UINT64 SegmentSize;
} VMEM_BOUNDARY_TAG, *PVMEM_BOUNDARY_TAG;

typedef struct _VMEM_ARENA
{
    LPCSTR Name;
    UINT_PTR Base;
    UINT64 Size;
    UINT64 Quantum;
    UINT_PTR (*AllocFunction)(_VMEM_ARENA*, UINT64);
    VOID (*FreeFunction)(_VMEM_ARENA*, VOID*, UINT64);
    _VMEM_ARENA *Source;
    // TODO: Implement Quantum Caching
    LIST<VMEM_BOUNDARY_TAG> SegmentList;
    // TODO: Add hash allocation chain
    LIST<VMEM_BOUNDARY_TAG> Freelists[64];
} VMEM_ARENA, *PVMEM_ARENA;

namespace Mm
{
    INT VmemCreateArena(PVMEM_ARENA Arena,
                    LPCSTR Name,
                    UINT_PTR Base,
                    UINT64 Size,
                    UINT64 Quantum,
                    UINT_PTR (*AllocFunction)(PVMEM_ARENA, UINT64),
                    VOID (*FreeFunction)(_VMEM_ARENA*, VOID*, UINT64),
                    PVMEM_ARENA Source);

    UINT_PTR VmemAllocate(PVMEM_ARENA Arena, UINT64 Size);
    VOID VmemFree(PVMEM_ARENA Arena, UINT_PTR Address, UINT64 Size);
};