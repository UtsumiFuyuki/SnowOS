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
#include <hal/paging.hpp>
#include <ke/log.hpp>

// Bootstrao Heap Arena
VMEM_ARENA heapArena{};
VMEM_ARENA userArena{};

void mm::initializeVmm() {
    ke::log(__FILE__, "Initializing Virtual Memory Manager...\r\n");
    mm::vmemCreateArena(&heapArena, "heapArena", 0xFFFFA00000000000, 0xF000000000000000, 0x1000, nullptr, nullptr, nullptr);
    mm::vmemCreateArena(&userArena, "userArena", 0x1000, 0xF000000000000000, 0x1000, nullptr, nullptr, nullptr);

    auto *currentNode = heapArena.segmentList.getHead();

    while (currentNode != nullptr) {
        ke::log(__FILE__, "[Type: %u, Base: 0x%llX, Size: 0x%llX]\r\n",
        currentNode->data.type,
        currentNode->data.segmentBase,
        currentNode->data.segmentSize);
        currentNode = currentNode->next;
    }
    ke::print("Initialized Virtual Memory Manager!\r\n");
}

void *mm::allocateKernelPages(size_t pages) {
    if (pages == 0) {
        return nullptr;
    }

    uintptr_t virtualAddress = mm::vmemAllocate(&heapArena, pages * 0x1000);

    for (size_t i = 0; i < pages * 0x1000; i += 0x1000) {
        uintptr_t page = mm::allocatePage();
        hal::mapPage(page, virtualAddress + i, PAGE_WRITE);
    }

    return reinterpret_cast<void *>(virtualAddress);
}

uintptr_t mm::allocateKernelVirt(size_t pages) {
    return mm::vmemAllocate(&heapArena, pages * 0x1000);
}

void mm::freeKernelVirt(uintptr_t address, size_t pages) {
    mm::vmemFree(&heapArena, address, pages * 0x1000);
}

void *mm::allocateUserPages(size_t pages) {
    if (pages == 0) {
        return nullptr;
    }

    uintptr_t virtualAddress = mm::vmemAllocate(&userArena, pages * 0x1000);

    for (size_t i = 0; i < pages * 0x1000; i += 0x1000) {
        uintptr_t page = mm::allocatePage();
        hal::mapPage(page, virtualAddress + i, PAGE_WRITE | PAGE_USER);
    }

    return reinterpret_cast<void *>(virtualAddress);
}

uintptr_t mm::allocateUserVirt(size_t pages) {
    return mm::vmemAllocate(&userArena, pages * 0x1000);
}

void mm::freeUserVirt(uintptr_t address, size_t pages) {
    mm::vmemFree(&userArena, address, pages * 0x1000);
}