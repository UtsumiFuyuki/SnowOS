/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/vmem.cpp

Description:
An implementation of Vmem for Yuki

Author:
UtsumiFuyuki
March 28th 2026
**/

#include <mm/mm.hpp>
#include <mm/vmem.hpp>

INT Mm::VmemCreateArena(PVMEM_ARENA Out,
                    LPCSTR Name,
                    PVOID Base,
                    UINT64 Size,
                    UINT64 Quantum,
                    UINT_PTR (*AllocFunction)(PVMEM_ARENA, UINT64),
                    VOID (*FreeFunction)(_VMEM_ARENA*, VOID*, UINT64),
                    PVMEM_ARENA Source)
{
    Out->Name = Name;
    Out->Base = Base;
    Out->Size = Size;
    Out->Quantum = Quantum;
    Out->AllocFunction = AllocFunction;
    Out->FreeFunction = FreeFunction;
    Out->Source = nullptr;

    return 0;
}

UINT_PTR Mm::VmemAllocate(PVMEM_ARENA Arena, UINT64 Size)
{
    return 0;
}
