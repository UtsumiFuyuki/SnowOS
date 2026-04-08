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
VMEM_ARENA heapArena{};

void mm::initializeVmm() {
    ke::print("Initializing Virtual Memory Manager...\r\n");
    mm::vmemCreateArena(&heapArena, "heapArena", 0xFFFFB00000000000, 0xF0000000, 0x1000, nullptr, nullptr, nullptr);

    auto *currentNode = heapArena.segmentList.getHead();

    while (currentNode != nullptr) {
        ke::log(__FILE__, "[Type: %u, Base: 0x%llX, Size: 0x%llX]\r\n",
        currentNode->data.type,
        currentNode->data.segmentBase,
        currentNode->data.segmentSize);
        currentNode = currentNode->next;
    }
    
    uintptr_t va = mm::vmemAllocate(&heapArena, 2);

    currentNode = heapArena.segmentList.getHead();

    while (currentNode != nullptr) {
        ke::log(__FILE__, "[Type: %u, Base: 0x%llX, Size: 0x%llX]\r\n",
        currentNode->data.type,
        currentNode->data.segmentBase,
        currentNode->data.segmentSize);
        currentNode = currentNode->next;
    }

    ke::log(__FILE__, "Allocated virtual address is 0x%llX\r\n", va);
}

