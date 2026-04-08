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

void hal::initializePaging() {
    #if defined (__x86_64__)
            hal::x64::initializePaging();
    #endif
}

uint64_t hal::generalFlagsToArchSpecific(uint64_t flags) {
    uint64_t ArchFlags{};

    if (flags & PAGE_WRITE) {
        #if defined (__x86_64__)
            ArchFlags |= PTE_WRITE;
        #endif
    }

    if (flags & PAGE_USER) {
        #if defined (__x86_64__)
            ArchFlags |= PTE_USER;
        #endif
    }

    if (flags & PAGE_NO_EXECUTE) {
        #if defined (__x86_64__)
            ArchFlags |= PTE_EXECUTE_DISABLE;
        #endif
    }

    return ArchFlags;
}

void hal::mapPage(uintptr_t physicalAddress, uintptr_t virtualAddress, uint64_t flags) {
    flags = hal::generalFlagsToArchSpecific(flags);

    #if defined (__x86_64__)
            hal::x64::mapPage(physicalAddress, virtualAddress, flags);
    #endif
}

void hal::mapPages(uintptr_t physicalAddress, uintptr_t virtualAddress, uint64_t Length, uint64_t Flags) {
    Flags = hal::generalFlagsToArchSpecific(Flags);

    #if defined (__x86_64__)
            hal::x64::mapPages(physicalAddress, virtualAddress, Length, Flags);
    #endif
}

void hal::unmapPage(uintptr_t virtualAddress) {
    #if defined (__x86_64__)
            hal::x64::unmapPage(virtualAddress);
    #endif
}

void hal::unmapPages(uintptr_t virtualAddress, uint64_t Length) {
    #if defined (__x86_64__)
            hal::x64::unmapPages(virtualAddress, Length);
    #endif
}

uintptr_t hal::virtualToPhysical(uintptr_t virtualAddress) {
    #if defined (__x86_64__)
        return hal::x64::virtualToPhysical(virtualAddress);
    #endif
    return 0;
}