#pragma once

#include <cstdint>

#define PML4_SHIFT 39
#define PDPT_SHIFT 30
#define PD_SHIFT 21
#define PT_SHIFT 12

#define PML4_ID(virt) (((virt) >> PML4_SHIFT) & 0x1FF)
#define PDPT_ID(virt) (((virt) >> PDPT_SHIFT) & 0x1FF)
#define PD_ID(virt) (((virt) >> PD_SHIFT) & 0x1FF)
#define PT_ID(virt) (((virt) >> PT_SHIFT) & 0x1FF)

#define PT_ADDR 0x0000FFFFFFFFF000

#define PTE_PRESENT (1ULL << 0)
#define PTE_WRITE (1ULL << 1)
#define PTE_USER (1ULL << 2)
#define PTE_PWT (1ULL << 3)
#define PTE_PCD (1ULL << 4)
#define PTE_EXECUTE_DISABLE (1ULL << 63)

typedef uint64_t PAGE_TABLE_ENTRY;

extern "C" uint64_t *kernelPml4;

namespace hal {
    namespace x64 {
        void initializePaging();
        void setCr3(uintptr_t pml4);
        PAGE_TABLE_ENTRY createNewEntry(uint64_t flags);
        void mapPage(uintptr_t physicalAddress, uintptr_t virtualAddress, uint64_t flags, uint64_t *pagemap = kernelPml4);
        void mapPages(uintptr_t physicalAddress, uintptr_t virtualAddress, uint64_t length, uint64_t flags, uint64_t *pagemap = kernelPml4); 
        void unmapPage(uintptr_t virtualAddress, uint64_t *pagemap = kernelPml4);
        void unmapPages(uintptr_t virtualAddress, uint64_t length, uint64_t *pagemap = kernelPml4);
        uintptr_t virtualToPhysical(uint64_t virtualAddress, uint64_t *pagemap = kernelPml4);

        uint64_t *kernelPagemap();
    }
}