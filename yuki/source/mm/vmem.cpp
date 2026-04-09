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

// Statically allocated nodes for bootstrap
LL_NODE<VMEM_BOUNDARY_TAG> initNodes[64]{};
size_t nextFreeNode{};

uint64_t pow(uint64_t base, uint64_t exponent) {
    uint64_t ret{1};

    for (size_t i = 1; i <= exponent; i++) {
        ret *= base;
    }
    return ret;
}

int mm::vmemCreateArena(PVMEM_ARENA arena,
                    const char *name,
                    uintptr_t base,
                    size_t size,
                    uint64_t quantum,
                    uintptr_t (*allocFunction)(PVMEM_ARENA, size_t),
                    void (*freeFunction)(_VMEM_ARENA*, void*, size_t),
                    PVMEM_ARENA source) {
    arena->name = name;
    arena->base = base;
    arena->size = size;
    arena->quantum = quantum;
    arena->allocFunction = allocFunction;
    arena->freeFunction = freeFunction;
    arena->source = source;

    initNodes[nextFreeNode].data = {.type = VMEM_SEGMENT_SPAN, .segmentBase = base, .segmentSize = size};
    arena->segmentList.push(&initNodes[nextFreeNode]);
    nextFreeNode++;

    initNodes[nextFreeNode].data = {.type = VMEM_SEGMENT_FREE, .segmentBase = base, .segmentSize = size};
    arena->segmentList.push(&initNodes[nextFreeNode]);

    for (size_t i = 0; i < 64; i++) {
        if (pow(2, i + 1) >= size) {
            ke::log(__FILE__, "Size 0x%llX goes onto freelist %llu\r\n", size, i);
            arena->freelists[i].push(&initNodes[nextFreeNode]);
            break;
        }
    }
    nextFreeNode++;

    return 0;
}

// TODO: Implement NEXTFIT and BESTFIT
uintptr_t mm::vmemAllocate(PVMEM_ARENA arena, size_t size) {
    // TODO: We assume the client has provided a quantum-aligned size, properly round to quantum if it's not aligned
    
    if (arena == nullptr)
        return 0;

    //TODO: Use a log2 operation instead =P
    for (size_t n = 0; n < 64; n++) {
        if (!arena->freelists[n].empty() && pow(2, n + 1) >= size) {
            auto *head = arena->freelists[n].getHead();
            head->data.segmentSize -= size;

            if (head->data.segmentSize == 0) {
                arena->freelists[n].remove(head);
            }

            for (auto *currentNode = arena->segmentList.getHead(); currentNode != nullptr; currentNode = currentNode->next) {
                // Find corresponding node in segment list
                if (currentNode->data.segmentBase == head->data.segmentBase && currentNode->data.type != VMEM_SEGMENT_SPAN) {
                    if (currentNode->data.segmentSize == size) {
                        currentNode->data.type = VMEM_SEGMENT_ALLOCATED;
                        return currentNode->data.segmentBase;
                    }
                    auto *allocatedNode = &initNodes[nextFreeNode];
                    allocatedNode->data.segmentSize = size;
                    allocatedNode->data.segmentBase = currentNode->data.segmentBase;
                    allocatedNode->data.type = VMEM_SEGMENT_ALLOCATED;

                    currentNode->data.segmentBase += size;

                    arena->segmentList.insert(currentNode, allocatedNode);
                    nextFreeNode++;
                    return allocatedNode->data.segmentBase;
                }
            }
        }
    }

    return 0;
}

void mm::vmemFree(PVMEM_ARENA arena, uintptr_t address, size_t size) {
    // TODO: We assume the client has provided a quantum-aligned size, properly round to quantum if it's not aligned

    for (auto *currentNode = arena->segmentList.getHead(); currentNode != nullptr; currentNode = currentNode->next) {
        if (currentNode->data.segmentBase <= (address & ~0xFFF) && currentNode->data.segmentBase + currentNode->data.segmentSize > (address & ~0xFFF)) {
            
            if (currentNode->data.type == VMEM_SEGMENT_SPAN)
                continue;

            auto *freedNode = &initNodes[nextFreeNode];
            nextFreeNode++;
            freedNode->data = {.type = VMEM_SEGMENT_FREE, .segmentBase = (address & ~0xFFF), .segmentSize = size};

            auto newSize = freedNode->data.segmentBase - currentNode->data.segmentBase;
            auto remainingSize = currentNode->data.segmentSize - newSize - size;

            ke::log(__FILE__, "New size of node is 0x%llX Remaining size is 0x%llX\r\n", newSize, remainingSize);
            
            if (currentNode->data.segmentBase == freedNode->data.segmentBase) {
                currentNode->data.segmentBase += size;
                currentNode->data.segmentSize -= size;
                arena->segmentList.insert(currentNode, freedNode);

                if (currentNode->data.segmentSize == 0)
                {
                    arena->segmentList.remove(currentNode);
                }
            }
            else {
                currentNode->data.segmentSize = newSize;
                arena->segmentList.insert(currentNode, freedNode, LIST_INSERT_AFTER);

                if (remainingSize >= 0) {
                    auto *newNode = &initNodes[nextFreeNode];
                    nextFreeNode++;
                    newNode->data = {.type = VMEM_SEGMENT_ALLOCATED, .segmentBase = (address & ~0xFFF) + size, .segmentSize = remainingSize};
                    arena->segmentList.insert(freedNode, newNode, LIST_INSERT_AFTER);
                }
            }

            // TODO: Modify Freelist

            // Coalesce blocks together
            // Can probably move this someplace else so I don't end up allocating nodes when they're gonna be coalesced anyways
            if (freedNode->next != nullptr && freedNode->next->data.type == VMEM_SEGMENT_FREE) {
                auto *nextFree = freedNode->next;
                if (nextFree->data.segmentBase == freedNode->data.segmentBase + freedNode->data.segmentSize) {
                    freedNode->data.segmentSize += nextFree->data.segmentSize;
                    arena->segmentList.remove(nextFree);
                }
            }

            if (freedNode->prev != nullptr && freedNode->prev->data.type == VMEM_SEGMENT_FREE) {
                auto *prevFree = freedNode->prev;
                if (prevFree->data.segmentBase + prevFree->data.segmentSize == freedNode->data.segmentBase) {
                    freedNode->data.segmentSize += prevFree->data.segmentSize;
                    freedNode->data.segmentBase -= prevFree->data.segmentSize;
                    arena->segmentList.remove(prevFree);
                }
            }

            break;
        }
    }
}
