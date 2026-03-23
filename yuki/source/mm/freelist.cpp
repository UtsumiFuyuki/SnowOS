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