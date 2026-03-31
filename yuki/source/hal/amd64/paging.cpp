/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/amd64/paging.cpp

Description:
This file contains amd64 specific paging functions

Author:
UtsumiFuyuki
March 20th 2026
**/

#include <hal/hal.hpp>
#include <hal/amd64/paging.hpp>
#include <ke/string.hpp>
#include <ke/log.hpp>
#include <mm/early_alloc.hpp>

UINT64 *KernelPml4{};

VOID Hal::X64::InitializePaging()
{
    UINT64 Cr3;
    __asm__ volatile ("mov %%cr3, %0" : "=a"(Cr3));

    KernelPml4 = reinterpret_cast<UINT64 *>(Cr3 + Hal::RetrieveHhdmOffset());
}

VOID Hal::X64::SetCr3(UINT_PTR Pml4)
{
    __asm__ volatile ("mov %0, %%cr3" :: "a"(Pml4));
    KernelPml4 = reinterpret_cast<UINT64 *>(Pml4 + Hal::RetrieveHhdmOffset());
}

PAGE_TABLE_ENTRY Hal::X64::CreateNewEntry(UINT64 Flags)
{
    UINT64 PageTable = Mm::EarlyAllocatePage();
    memset(reinterpret_cast<UINT64 *>(PageTable + Hal::RetrieveHhdmOffset()), 0, 0x1000);
    return (PageTable | PTE_PRESENT | Flags);
}

VOID Hal::X64::MapPage(UINT_PTR PhysicalAddress, UINT_PTR VirtualAddress, UINT64 Flags)
{
    UINT64 *Level = KernelPml4;

    if ((PhysicalAddress & 0xFFF) != 0 || (VirtualAddress & 0xFFF) != 0)
    {
        Ke::Log(__FILE__, "PA 0x%llX or VA 0x%llX is not page-aligned!\r\n", PhysicalAddress, VirtualAddress);
        return;
    }

    for (UINT64 i = 39; i >= 12; i -= 9)
    {
        PAGE_TABLE_ENTRY Entry = Level[((VirtualAddress >> i) & 0x1FF)];

        if (!(Entry & PTE_PRESENT))
        {
            if (i == PT_SHIFT)
            {
                Entry = (PhysicalAddress | PTE_PRESENT | Flags);
            }

            else
            {
                // The higher page table entries should be more permissive
                Entry = Hal::X64::CreateNewEntry(PTE_WRITE | PTE_USER);
            }

            Level[((VirtualAddress >> i) & 0x1FF)] = Entry;
        }

        Level = reinterpret_cast<UINT64 *>((Entry & PT_ADDR) + Hal::RetrieveHhdmOffset());
    }

    //Ke::Log(__FILE__, "Physical Address 0x%llX has been mapped to Virtual Address 0x%llX\r\n", PhysicalAddress, VirtualAddress);
}

VOID Hal::X64::MapPages(UINT_PTR PhysicalAddress, UINT_PTR VirtualAddress, UINT64 Length, UINT64 Flags)
{
    if ((PhysicalAddress & 0xFFF) != 0 || (VirtualAddress & 0xFFF) != 0 || (Length & 0xFFF) != 0)
    {
        Ke::Log(__FILE__, "PA 0x%llX, VA 0x%llX or Length 0x%llX is not page-aligned!\r\n", PhysicalAddress, VirtualAddress, Length);
        return;
    }

    for (UINT64 i = 0; i < Length; i += 0x1000)
    {
        MapPage(PhysicalAddress + i, VirtualAddress + i, Flags);
    }
}

VOID Hal::X64::UnmapPage(UINT_PTR VirtualAddress)
{
    if ((VirtualAddress & 0xFFF) != 0)
    {
        Ke::Log(__FILE__, "VA 0x%llX is not page-aligned!\r\n", VirtualAddress);
        return;
    }

    else if (VirtualToPhysical(VirtualAddress) == 0)
    {
        Ke::Log(__FILE__, "0x%llX does not have a mapping! Can't unmap!\r\n");
        return;
    }

    UINT64 *Level = KernelPml4;

    for (UINT64 i = 39; i >= 12; i -= 9)
    {
        PAGE_TABLE_ENTRY Entry = Level[((VirtualAddress >> i) & 0x1FF)];

        if (i == PT_SHIFT)
        {
            Entry = 0;
            Level[((VirtualAddress >> i) & 0x1FF)] = Entry;
            break;
        }

        Level = reinterpret_cast<UINT64 *>((Entry & PT_ADDR) + Hal::RetrieveHhdmOffset());
    }

    __asm__ volatile ("invlpg (%0)" :: "a"(VirtualAddress));
}

VOID Hal::X64::UnmapPages(UINT_PTR VirtualAddress, UINT64 Length)
{
    if ((Length & 0xFFF) != 0)
    {
        Ke::Log(__FILE__, "Length 0x%llX is not page-aligned!\r\n", Length);
        return; 
    }

    for (UINT64 i = 0; i < Length; i += 0x1000)
    {
        UnmapPage(VirtualAddress + i);
    }
}

UINT_PTR Hal::X64::VirtualToPhysical(UINT_PTR VirtualAddress)
{
    UINT64 *Level = KernelPml4;

    for (UINT64 i = 39; i >= 12; i -= 9)
    {
        PAGE_TABLE_ENTRY Entry = Level[((VirtualAddress >> i) & 0x1FF)];

        if (!(Entry & PTE_PRESENT))
        {
            return 0;
        }

        if (i == PT_SHIFT)
        {
            return (Entry & PT_ADDR);
        }

        Level = reinterpret_cast<UINT64 *>((Entry & PT_ADDR) + Hal::RetrieveHhdmOffset());
    }
}