/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/vm_init.cpp

Description:
Virtual memory managment initialization code

Author:
UtsumiFuyuki
April 2nd 2026
**/

#include <mm/mm.hpp>
#include <mm/vmem.hpp>
#include <ke/log.hpp>

// Bootstrao Heap Arena
VMEM_ARENA HeapArena{};

VOID Mm::InitializeVmm()
{
    Ke::Print("Initializing Virtual Memory Manager...\r\n");
    Mm::VmemCreateArena(&HeapArena, "HeapArena", 0xFFFFB00000000000, 0xF0000000, 0x1000, nullptr, nullptr, nullptr);
    Ke::Log(__FILE__, "Name: %s Base: 0x%llX Size: 0x%llX Quantum: 0x%llX\r\n", HeapArena.Name, HeapArena.Base, HeapArena.Size, HeapArena.Quantum);

    auto *CurrentNode = HeapArena.SegmentList.GetHead();

    while (CurrentNode != nullptr)
    {
        Ke::Log(__FILE__, "[Type: %u, Base: 0x%llX, Size: 0x%llX]\r\n",
        CurrentNode->Data.Type,
        CurrentNode->Data.SegmentStart,
        CurrentNode->Data.SegmentSize);
        CurrentNode = CurrentNode->Next;
    }
    
    uintptr_t va = Mm::VmemAllocate(&HeapArena, 2);

    CurrentNode = HeapArena.SegmentList.GetHead();

    while (CurrentNode != nullptr)
    {
        Ke::Log(__FILE__, "[Type: %u, Base: 0x%llX, Size: 0x%llX]\r\n",
        CurrentNode->Data.Type,
        CurrentNode->Data.SegmentStart,
        CurrentNode->Data.SegmentSize);
        CurrentNode = CurrentNode->Next;
    }

    Ke::Log(__FILE__, "Allocated virtual address is 0x%llX\r\n", va);
}