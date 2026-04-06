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

#include "utils/list.hpp"
#include <mm/mm.hpp>
#include <mm/vmem.hpp>
#include <ke/log.hpp>
#include <ke/string.hpp>

// Statically allocated ll nodes for bootstrap
LL_NODE<VMEM_BOUNDARY_TAG> InitNodes[16]{};
size_t NextFreeNode{};

uint64_t Pow(uint64_t Base, uint64_t Exponent)
{
    uint64_t Ret{1};

    for (size_t i = 1; i <= Exponent; i++)
    {
        Ret *= Base;
    }
    return Ret;
}

INT Mm::VmemCreateArena(PVMEM_ARENA Out,
                    LPCSTR Name,
                    UINT_PTR Base,
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
    Out->Source = Source;

    InitNodes[NextFreeNode].Data = {.Type = VMEM_SEGMENT_SPAN, .SegmentStart = Base, .SegmentSize = Size};
    Out->SegmentList.Push(&InitNodes[NextFreeNode]);
    NextFreeNode++;

    InitNodes[NextFreeNode].Data = {.Type = VMEM_SEGMENT_FREE, .SegmentStart = Base, .SegmentSize = Size};
    Out->SegmentList.Push(&InitNodes[NextFreeNode]);

    for (int i = 0; i < 64; i++)
    {
        if (Pow(2, i + 1) >= Size)
        {
            Ke::Log(__FILE__, "Size 0x%llX goes onto freelist %llu\r\n", Size, i);
            Out->Freelists[i].Push(&InitNodes[NextFreeNode]);
            break;
        }
    }
    NextFreeNode++;

    return 0;
}

// TODO: Implement NEXTFIT and BESTFIT
UINT_PTR Mm::VmemAllocate(PVMEM_ARENA Arena, UINT64 Size)
{
    if (Arena == nullptr)
        return 0;

    // TODO: Change this to properly round to the quantum
    Size *= Arena->Quantum;

    for (size_t n = 0; n < 64; n++)
    {
        if (!Arena->Freelists[n].Empty() && Pow(2, n + 1) >= Size)
        {
            auto *Head = Arena->Freelists[n].GetHead();
            Head->Data.SegmentSize -= Size;

            if (Head->Data.SegmentSize == 0)
            {
                Arena->Freelists[n].Remove(Head);
            }

            for (auto *CurrentNode = Arena->SegmentList.GetHead(); CurrentNode != nullptr; CurrentNode = CurrentNode->Next)
            {
                // Find corresponding node in segment list
                if (CurrentNode->Data.SegmentStart == Head->Data.SegmentStart && CurrentNode->Data.Type != VMEM_SEGMENT_SPAN)
                {
                    if (CurrentNode->Data.SegmentSize == Size)
                    {
                        CurrentNode->Data.Type = VMEM_SEGMENT_ALLOCATED;
                        return CurrentNode->Data.SegmentStart;
                    }
                    auto *AllocatedNode = &InitNodes[NextFreeNode];
                    AllocatedNode->Data.SegmentSize = Size;
                    AllocatedNode->Data.SegmentStart = CurrentNode->Data.SegmentStart;
                    AllocatedNode->Data.Type = VMEM_SEGMENT_ALLOCATED;

                    CurrentNode->Data.SegmentStart += Size;

                    Arena->SegmentList.Insert(CurrentNode, AllocatedNode);
                    NextFreeNode++;
                    return AllocatedNode->Data.SegmentStart;
                }
            }
        }
    }

    return 0;
}
