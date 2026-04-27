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

#include <cstdint>
#include <hal/paging.hpp>
#include <hal/amd64/paging.hpp>
#include <mm/early_alloc.hpp>

uint64_t *kernelPagemap = nullptr;

void hal::initializePaging() {
    #if defined (__x86_64__)
            hal::x64::initializePaging();
            kernelPagemap = hal::getKernelPagemap();
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

    if (flags & PAGE_CACHE_DISABLE) {
        #if defined (__x86_64__)
            ArchFlags |= PTE_PCD;
        #endif
    }

    if (flags & PAGE_WRITE_THROUGH) {
        #if defined (__x86_64__)
            ArchFlags |= PTE_PWT;
        #endif
    }

    return ArchFlags;
}

void hal::mapPage(uintptr_t physicalAddress, uintptr_t virtualAddress, uint64_t flags, uint64_t *pagemap) {
    flags = hal::generalFlagsToArchSpecific(flags);

    #if defined (__x86_64__)
            hal::x64::mapPage(physicalAddress, virtualAddress, flags, pagemap);
    #endif
}

void hal::mapPages(uintptr_t physicalAddress, uintptr_t virtualAddress, uint64_t Length, uint64_t Flags, uint64_t *pagemap) {
    Flags = hal::generalFlagsToArchSpecific(Flags);

    #if defined (__x86_64__)
            hal::x64::mapPages(physicalAddress, virtualAddress, Length, Flags, pagemap);
    #endif
}

void hal::unmapPage(uintptr_t virtualAddress, uint64_t *pagemap) {
    #if defined (__x86_64__)
            hal::x64::unmapPage(virtualAddress, pagemap);
    #endif
}

void hal::unmapPages(uintptr_t virtualAddress, uint64_t Length, uint64_t *pagemap) {
    #if defined (__x86_64__)
            hal::x64::unmapPages(virtualAddress, Length, pagemap);
    #endif
}

uintptr_t hal::virtualToPhysical(uintptr_t virtualAddress, uint64_t *pagemap) {
    #if defined (__x86_64__)
        return hal::x64::virtualToPhysical(virtualAddress, pagemap);
    #endif
    return 0;
}

uint64_t *hal::getKernelPagemap() {
    #if defined (__x86_64__)
        return hal::x64::kernelPagemap();
    #endif
    return nullptr;
}