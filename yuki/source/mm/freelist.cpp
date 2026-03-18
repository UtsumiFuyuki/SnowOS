/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/freelist.cpp

Description:
This file contains the main freelist allocator code

Author:
UtsumiFuyuki
February 5th 2026 
**/

#include <typedefs.hpp>
#include <limine.h>
#include <ke/string.hpp>
#include <hal/hal.hpp>
#include <ke/log.hpp>
#include <mm/freelist.hpp>

FREELIST_NODE FreelistHead = {.Start = 0, .Length = 0, .Next = nullptr};

VOID Mm::InitializeFreelist(limine_memmap_response *MemoryMap)
{
    ULONG64 NumberOfPages = 0;

    BOOL EndOfMemory = true;

    FREELIST_NODE *CurrentNode = &FreelistHead;
    FREELIST_NODE *NextNode;

    if (MemoryMap == nullptr)
    {
        Ke::Print("MemoryMap passed to MmInitializeFreelist is NULL!\n");
    }

    for (UINT64 i = 0; i < MemoryMap->entry_count; i++)
    {
        //KePrint("Memory Map Entry: %lu | Base: 0x%lX | Length 0x%lX\n", i, MemoryMap->entries[i]->base, MemoryMap->entries[i]->length);

        if (MemoryMap->entries[i]->type == LIMINE_MEMMAP_USABLE)
        {
            NumberOfPages += (MemoryMap->entries[i]->length / 0x1000);

            // Find next usable chunk of memory
            for (UINT64 j = i + 1; j < MemoryMap->entry_count; j++)
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

    Ke::Print("Usable System Memory Detected: %luMiB\n", ((NumberOfPages * 4) / 1024));
}

UINT_PTR Mm::AllocatePhysicalPage()
{
    UINT_PTR Address = FreelistHead.Start;

    FreelistHead.Start += 0x1000;
    FreelistHead.Length -= 0x1000;

    if (FreelistHead.Length == 0)
    {
        FreelistHead = *FreelistHead.Next;
    }
    
    return Address;
}

VOID Mm::FreePhysicalPage(UINT_PTR Page)
{
    FREELIST_NODE *NewNode = reinterpret_cast<FREELIST_NODE *>(Page + Hal::RetrieveHhdmOffset());
    memset(NewNode, 0, 0x1000);
    NewNode->Next = &FreelistHead;
    NewNode->Start = Page;
    NewNode->Length = 0x1000;
    FreelistHead = *NewNode;
}