/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/paging.cpp

Description:
This file contains amd64 specific paging functions

Author:
UtsumiFuyuki
March 20th 2026
**/

#include <hal/paging.hpp>
#include <hal/amd64/paging.hpp>
#include <mm/early_alloc.hpp>

VOID Hal::InitializePaging()
{
    #if defined (__x86_64__)
            Hal::X64::InitializePaging();
    #endif
}

UINT64 Hal::GeneralFlagsToArchSpecific(UINT64 Flags)
{
    UINT64 ArchFlags{};

    if (Flags & PAGE_WRITE)
    {
        #if defined (__x86_64__)
            ArchFlags |= PTE_WRITE;
        #endif
    }

    if (Flags & PAGE_USER)
    {
        #if defined (__x86_64__)
            ArchFlags |= PTE_USER;
        #endif
    }

    if (Flags & PAGE_NO_EXECUTE)
    {
        #if defined (__x86_64__)
            ArchFlags |= PTE_EXECUTE_DISABLE;
        #endif
    }

    return ArchFlags;
}

VOID Hal::MapPage(UINT_PTR PhysicalAddress, UINT_PTR VirtualAddress, UINT64 Flags)
{
    Flags = Hal::GeneralFlagsToArchSpecific(Flags);

    #if defined (__x86_64__)
            Hal::X64::MapPage(PhysicalAddress, VirtualAddress, Flags);
    #endif
}

VOID Hal::MapPages(UINT_PTR PhysicalAddress, UINT_PTR VirtualAddress, UINT64 Length, UINT64 Flags)
{
    Flags = Hal::GeneralFlagsToArchSpecific(Flags);

    #if defined (__x86_64__)
            Hal::X64::MapPages(PhysicalAddress, VirtualAddress, Length, Flags);
    #endif
}

VOID Hal::UnmapPage(UINT_PTR VirtualAddress)
{
    #if defined (__x86_64__)
            Hal::X64::UnmapPage(VirtualAddress);
    #endif
}

VOID Hal::UnmapPages(UINT_PTR VirtualAddress, UINT64 Length)
{
    #if defined (__x86_64__)
            Hal::X64::UnmapPages(VirtualAddress, Length);
    #endif
}

UINT_PTR Hal::VirtualToPhysical(UINT_PTR VirtualAddress)
{
    #if defined (__x86_64__)
        return Hal::X64::VirtualToPhysical(VirtualAddress);
    #endif
    return 0;
}