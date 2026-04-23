/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/slab.cpp

Description:
Slab implementation for Yuki
NOTE: This implementation does not include object caching at present time
We also don't support large slabs... or slab colouring
I mostly just wanna work on scheduling, I'll come back right to this I promise =P

Author:
UtsumiFuyuki
April 15th, 2026
**/

#include <cstdint>
#include <mm/mm.hpp>
#include <mm/slab.hpp>
#include <ke/log.hpp>

#define SLAB_SMALL_MAX_SIZE 512

// We currently only cache object sizes, I'll implement proper
// object caching... at some point
uint64_t cacheCount{}; // Number of caches
SLAB_CACHE *caches{nullptr}; // Array of caches

void mm::initializeSlab() {
    ke::log(__FILE__, "Allocating page for cache list...\r\n");
    caches = reinterpret_cast<SLAB_CACHE *>(mm::allocateKernelPages(1));

    if (caches == nullptr) {
        ke::log(__FILE__, "Failed to allocate pages for cache list!\r\n");
        return;
    }

    ke::log(__FILE__, "caches start at 0x%llX\r\n", caches);
    
    size_t i{};
    for (size_t n = 8; n <= 512; n *= 2) {
        caches[i].cacheSize = n;
        auto *slab = createSlabSmall(caches[i].cacheSize);
        slab->next = slab;
        slab->prev = slab;
        caches[i].list = slab;
        caches[i].freelist = slab;

        cacheCount++;
        i++;

        ke::log(__FILE__, "Created cache for %llu sized objects!\r\n", n);
    }
    ke::print("Slab allocator initialized!\r\n");
}

SLAB_CACHE *mm::createCache(size_t objectSize) {
    caches[cacheCount].cacheSize = objectSize;

    auto *slab = createSlabSmall(objectSize);
    slab->next = slab;
    slab->prev = slab;
    caches[cacheCount].list = slab;
    caches[cacheCount].freelist = slab;
    cacheCount++;

    return &caches[cacheCount];
}

SLAB *mm::createSlabSmall(size_t objectSize) {
    if (objectSize < 8) {
        ke::log(__FILE__, "Cannot create a slab with an objectSize less than 8! Rounding to 8!\r\n");
        objectSize = 8;
    }

    else if (objectSize > SLAB_SMALL_MAX_SIZE) {
        ke::log(__FILE__, "Cannot create a small slab with a size greater than 512! Please use createSlabLarge() instead!\r\n");
        return nullptr;
    }

    uint8_t *page = reinterpret_cast<uint8_t *>(mm::allocateKernelPages(1));

    if (page == nullptr) {
        ke::log(__FILE__, "Failed to allocate page for slab!\r\n");
        return nullptr;
    }

    SLAB *slab = reinterpret_cast<SLAB *>(page + (0x1000 - sizeof(SLAB)));
    slab->bufctlSmall = reinterpret_cast<SLAB::BUFCTL_SMALL *>(page);

    for (size_t i = 0; reinterpret_cast<uint64_t>(slab->bufctlSmall) + i < reinterpret_cast<uint64_t>(slab); i += objectSize) {
        slab->bufctlSmall->next = reinterpret_cast<SLAB::BUFCTL_SMALL *>(page + i);
        slab->bufctlSmall = slab->bufctlSmall->next;
    }
    slab->bufctlSmall->next = nullptr;
    slab->bufctlSmall = reinterpret_cast<SLAB::BUFCTL_SMALL *>(page);

    slab->objectSize = objectSize;
    slab->next = nullptr;
    slab->prev = nullptr;
    slab->refCount = 0;

    return slab;
}

// Allocates a buffer from a slab
void *mm::allocatePool(size_t numberOfBytes) {
    void *address{nullptr};

    if (numberOfBytes < 8)
        numberOfBytes = 8;

    // Find needed cache
    for (size_t i = 0; i < cacheCount; i++) {
        if (caches[i].cacheSize == numberOfBytes) {

            if (caches[i].freelist == nullptr) {
                // Create new slab
                ke::log(__FILE__, "No free slabs available!\r\n");

                mm::displayList(i);

                auto *slab = createSlabSmall(numberOfBytes);

                // Get tail of list
                auto *current = caches[i].list;
                for (; current->next != caches[i].list; current = current->next);

                slab->next = current->next;
                slab->prev = current;
                current->next = slab;

                caches[i].freelist = slab;
                caches[i].list->prev = slab;
                address = reinterpret_cast<void *>(slab->bufctlSmall);
                slab->bufctlSmall = slab->bufctlSmall->next;
                slab->refCount++;
            }

            else {
                auto *slab = caches[i].freelist;
                address = slab->bufctlSmall;
                slab->bufctlSmall = slab->bufctlSmall->next;;
                uint64_t oldRefCount = slab->refCount;
                slab->refCount++;

                if (slab->bufctlSmall == nullptr) {
                    // Move to full section of list
                    auto *prevSlab = slab->prev;
                    auto *nextSlab = slab->next;

                    prevSlab->next = nextSlab;
                    nextSlab->prev = prevSlab;

                    if (nextSlab == slab) {
                        caches[i].freelist = nullptr;
                    }

                    else {
                        bool freeSlabFound{false};
                        auto *current = nextSlab;
                        for (; current->next != caches[i].list; current = current->next) {
                            if (current->bufctlSmall != nullptr)
                                freeSlabFound = true;
                        }

                        if (!freeSlabFound)
                            caches[i].freelist = nullptr;
                    }

                    slab->next = caches[i].list;
                    slab->prev = caches[i].list->prev;
                    caches[i].list->prev = slab;

                    auto *current = caches[i].list;
                    for (; current->next != caches[i].list; current = current->next);
                    current->next = slab;

                    caches[i].list = slab;
                }

                else if (oldRefCount == 0) {
                    // Move to partial section
                    auto *current = caches[i].list;
                    for (; current->bufctlSmall == nullptr && current->next != caches[i].list; current = current->next);
                    slab->next = current->next;
                    current->next = slab;
                    slab->prev = current;
                }
            }
            return address;
        }
    }
    ke::log(__FILE__, "Cache of size %llu does not exist! Creating new one...\r\n", numberOfBytes);
    if (createCache(numberOfBytes) != nullptr) {
        ke::log(__FILE__, "New cache created at 0x%llX with size %llu!\r\n", &caches[cacheCount - 1], caches[cacheCount - 1].cacheSize);
        auto *slab = caches[cacheCount - 1].freelist;
        address = reinterpret_cast<void *>(slab->bufctlSmall);
        slab->bufctlSmall = slab->bufctlSmall->next;
    }
    return address;
}

// Frees a buffer back to the slab allocator
void mm::freePool(void *address) {
    uintptr_t page = (reinterpret_cast<uintptr_t>(address) & ~0xFFF);
    auto *slab = reinterpret_cast<SLAB *>(page + (0x1000 - sizeof(SLAB)));

    // Find cache to which the slab belongs
    size_t cache{};
    for (; cache < cacheCount; cache++) {
        if (caches[cache].cacheSize == slab->objectSize)
            break;
    }

    if (slab->bufctlSmall == nullptr) {
        ke::log(__FILE__, "Slab was FULL, moving to partial section!\r\n");
        auto *prevSlab = slab->prev;
        auto *nextSlab = slab->next;

        prevSlab->next = nextSlab;
        nextSlab->prev = prevSlab;

        bool inserted{false};
        auto *current = caches[cache].list;
        for (; current->next != caches[cache].list; current = current->next) {
            if (current->next->bufctlSmall != nullptr) {
                ke::log("", ";3\r\n");
                slab->next = current->next;
                slab->prev = current;
                current->next = slab;
                inserted = true;
                break;
            }
        }

        // Insert at tail
        if (!inserted) {
            slab->next = current->next;
            slab->prev = current;
            current->next = slab;
        }
        caches[cache].freelist = slab;
    }

    SLAB::BUFCTL_SMALL *bufctl = reinterpret_cast<SLAB::BUFCTL_SMALL *>(address);
    bufctl->next = slab->bufctlSmall;
    slab->bufctlSmall = bufctl;
    slab->refCount--;

    if (slab->refCount == 0) {
        ke::log(__FILE__, "Slab is now empty, moving to complete section!\r\n");
        auto *prevSlab = slab->prev;
        auto *nextSlab = slab->next;

        prevSlab->next = nextSlab;
        nextSlab->prev = prevSlab;

        auto *current = caches[cache].list;
        for (; current->next != caches[cache].list; current = current->next);

        slab->next = current->next;
        slab->prev = current;
        current->next = slab;
    }
}

void *operator new(size_t size) {
    return mm::allocatePool(size);
}

// I just use this for debugging, you can ignore it
void mm::displayList(size_t n) {
    auto *current = caches[n].list;
    for (; current->next != caches[n].list; current = current->next) {
        ke::log(__FILE__, "Prev 0x%llX Base 0x%llX Bufctl 0x%llX Refcount %llu Next 0x%llX\r\n", current->prev, current, current->bufctlSmall, current->refCount, current->next);
    }
    ke::log(__FILE__, "Prev 0x%llX Base 0x%llX Bufctl 0x%llX Refcount %llu  Next 0x%llX[TAIL]\r\n", current->prev, current, current->bufctlSmall, current->refCount, current->next);
}