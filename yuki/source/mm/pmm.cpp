/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/pmm.cpp

Description:
This file contains the main physical
memory allocation code

Author:
UtsumiFuyuki
February 5th 2026 
**/

#include <limine.h>
#include <cstdint>
#include <ke/string.hpp>
#include <hal/hal.hpp>
#include <ke/print.hpp>
#include <mm/pmm.hpp>

FREELIST_NODE FreelistHead = {.Start = 0, .Length = 0, .Next = nullptr};

void Mm::InitializeFreelist(limine_memmap_response *MemoryMap)
{
    uint64_t NumberOfPages = 0;

    bool EndOfMemory = true;

    FREELIST_NODE *CurrentNode = &FreelistHead;
    FREELIST_NODE *NextNode;

    if (MemoryMap == nullptr)
    {
        Ke::Print(LOG_TYPE::None, "MemoryMap passed to MmInitializeFreelist is NULL!\n");
    }

    for (uint64_t i = 0; i < MemoryMap->entry_count; i++)
    {
        //KePrint(LOG_TYPE::None, "Memory Map Entry: %lu | Base: 0x%lX | Length 0x%lX\n", i, MemoryMap->entries[i]->base, MemoryMap->entries[i]->length);

        if (MemoryMap->entries[i]->type == LIMINE_MEMMAP_USABLE)
        {
            NumberOfPages += (MemoryMap->entries[i]->length / 0x1000);

            // Find next usable chunk of memory
            for (uint64_t j = i + 1; j < MemoryMap->entry_count; j++)
            {
                if (MemoryMap->entries[j]->type == LIMINE_MEMMAP_USABLE)
                {
                    NextNode = reinterpret_cast<FREELIST_NODE *>(MemoryMap->entries[j]->base + Hal::RetrieveHhdmOffset());
                    memset(NextNode, 0, 0x1000);
                    EndOfMemory = false;
                    break;
                }
            }

            if (EndOfMemory)
            {
                CurrentNode->Next = nullptr;
            }
            else
            {
                CurrentNode->Next = NextNode;
                CurrentNode->Start = MemoryMap->entries[i]->base;
                CurrentNode->Length = MemoryMap->entries[i]->length;
                CurrentNode = NextNode;
            }

            EndOfMemory = true;
        }
    }

    Ke::Print(LOG_TYPE::None, "Usable System Memory Detected: %luMiB\n", ((NumberOfPages * 4) / 1024));
}

uintptr_t Mm::AllocatePhysicalPage()
{
    uintptr_t Address = FreelistHead.Start;

    FreelistHead.Start += 0x1000;
    FreelistHead.Length -= 0x1000;

    if (FreelistHead.Length == 0)
    {
        FreelistHead = *FreelistHead.Next;
    }
    
    return Address;
}

void Mm::FreePhysicalPage(uintptr_t Page)
{
    FREELIST_NODE *NewNode = reinterpret_cast<FREELIST_NODE *>(Page + Hal::RetrieveHhdmOffset());
    memset(NewNode, 0, 0x1000);
    NewNode->Next = &FreelistHead;
    NewNode->Start = Page;
    NewNode->Length = 0x1000;
    FreelistHead = *NewNode;
}