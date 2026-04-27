#pragma once

#include <cstdint>

#define PAGE_WRITE 0x1
#define PAGE_USER 0x2
#define PAGE_NO_EXECUTE 0x4
#define PAGE_CACHE_DISABLE 0x8
#define PAGE_WRITE_THROUGH 0x10

extern "C" uint64_t *kernelPagemap;

namespace hal {
    void initializePaging();
    void mapPage(uintptr_t physicalAddress, uintptr_t virtualAddress, uintptr_t flags, uint64_t *pagemap = kernelPagemap);
    void mapPages(uintptr_t physicalAddress, uintptr_t virtualAddress, uint64_t length, uint64_t flags, uint64_t *pagemap = kernelPagemap);
    void unmapPage(uintptr_t virtualAddress, uint64_t *pagemap = kernelPagemap);
    void unmapPages(uintptr_t virtualAddress, uint64_t Length, uint64_t *pagemap = kernelPagemap);
    uintptr_t virtualToPhysical(uintptr_t virtualAddress, uint64_t *pagemap = kernelPagemap);
    uint64_t generalFlagsToArchSpecific(uint64_t flags);

    uint64_t *getKernelPagemap();
}