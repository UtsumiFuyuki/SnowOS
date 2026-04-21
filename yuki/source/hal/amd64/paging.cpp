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

#include <cstdint>
#include <hal/hal.hpp>
#include <hal/amd64/paging.hpp>
#include <ke/string.hpp>
#include <ke/log.hpp>
#include <mm/early_alloc.hpp>
#include <ldr/pe.hpp>

uint64_t *kernelPml4{};

void hal::x64::initializePaging() {
    uint64_t cr3 = mm::earlyAllocatePage();
    memset(reinterpret_cast<uint64_t *>(cr3 + hal::retrieveHhdmOffset()), 0, 0x1000);
    kernelPml4 = reinterpret_cast<uint64_t *>(cr3 + hal::retrieveHhdmOffset());

    ke::log(__FILE__, "PML4 is located at 0x%llX\r\n", kernelPml4);

    // Map HHDM
    limine_memmap_response *memoryMap = hal::retrieveMemoryMap();
    for (size_t i = 0; i < memoryMap->entry_count; i++) {
        uint64_t flags = PTE_WRITE;
        if (memoryMap->entries[i]->type != LIMINE_MEMMAP_BAD_MEMORY &&
            memoryMap->entries[i]->type != LIMINE_MEMMAP_RESERVED &&
            memoryMap->entries[i]->type != LIMINE_MEMMAP_ACPI_NVS) {

            if (memoryMap->entries[i]->type == LIMINE_MEMMAP_FRAMEBUFFER)
                flags |= PTE_PWT;
            
            hal::x64::mapPages(memoryMap->entries[i]->base,
                memoryMap->entries[i]->base + hal::retrieveHhdmOffset(),
                memoryMap->entries[i]->length,
                flags);
        }
    }

    limine_file *yukiImage = hal::retrieveYukiImage();
    NT_HEADERS_64 *yukiHeader = ldr::parsePe(reinterpret_cast<uint8_t *>(yukiImage->address));

    hal::x64::mapPage(yukiPhysicalAddress(), yukiVirtualAddress(),
                        PTE_EXECUTE_DISABLE);

    for (size_t i = 0; i < yukiHeader->peHeader.numberOfSections; i++) {
        uint64_t flags{PTE_EXECUTE_DISABLE};
        if (yukiHeader->sectionTable[i].characteristics & IMAGE_SCN_MEM_EXECUTE) {
                flags = flags & ~PTE_EXECUTE_DISABLE;
            }

            if (yukiHeader->sectionTable[i].characteristics & IMAGE_SCN_MEM_WRITE) {
                flags |= PTE_WRITE;
            }

            uint64_t length = yukiHeader->sectionTable[i].virtualSize;
            if ((length & 0xFFF) != 0) {
                length = (length & ~0xFFF) + 0x1000;
            }

            hal::x64::mapPages(hal::yukiPhysicalAddress() + yukiHeader->sectionTable[i].virtualAddress,
                                hal::yukiVirtualAddress() + yukiHeader->sectionTable[i].virtualAddress, length, flags);
        }
    __asm__ volatile ("mov %0, %%cr3" :: "a"(cr3));

    ke::print("Paging Initialized!\r\n");
}

void hal::x64::setCr3(uintptr_t pml4) {
    __asm__ volatile ("mov %0, %%cr3" :: "a"(pml4));
    kernelPml4 = reinterpret_cast<uint64_t *>(pml4 + hal::retrieveHhdmOffset());
}

PAGE_TABLE_ENTRY hal::x64::createNewEntry(uint64_t flags) {
    PAGE_TABLE_ENTRY pageTable = mm::earlyAllocatePage();
    memset(reinterpret_cast<uint64_t *>(pageTable + hal::retrieveHhdmOffset()), 0, 0x1000);
    return (pageTable | PTE_PRESENT | flags);
}

void hal::x64::mapPage(uintptr_t physicalAddress, uintptr_t virtualAddress, uint64_t flags) {
    uint64_t *level = kernelPml4;

    if ((physicalAddress & 0xFFF) != 0 || (virtualAddress & 0xFFF) != 0) {
        ke::log(__FILE__, "PA 0x%llX or VA 0x%llX is not page-aligned!\r\n", physicalAddress, virtualAddress);
        return;
    }

    for (uint64_t i = 39; i >= 12; i -= 9) {
        PAGE_TABLE_ENTRY entry = level[((virtualAddress >> i) & 0x1FF)];

        if (!(entry & PTE_PRESENT)) {
            if (i == PT_SHIFT)
                entry = (physicalAddress | PTE_PRESENT | flags);
            else
                // The higher page table entries should be more permissive
                entry = hal::x64::createNewEntry(PTE_WRITE | PTE_USER);

            level[((virtualAddress >> i) & 0x1FF)] = entry;
        }

        level = reinterpret_cast<uint64_t *>((entry & PT_ADDR) + hal::retrieveHhdmOffset());
    }
}

void hal::x64::mapPages(uintptr_t physicalAddress, uintptr_t virtualAddress, uint64_t length, uint64_t flags) {
    if ((physicalAddress & 0xFFF) != 0 || (virtualAddress & 0xFFF) != 0 || (length & 0xFFF) != 0) {
        ke::log(__FILE__, "PA 0x%llX, VA 0x%llX or Length 0x%llX is not page-aligned!\r\n", physicalAddress, virtualAddress, length);
        return;
    }

    for (uint64_t i = 0; i < length; i += 0x1000) {
        mapPage(physicalAddress + i, virtualAddress + i, flags);
    }
}

void hal::x64::unmapPage(uintptr_t virtualAddress) {
    if ((virtualAddress & 0xFFF) != 0) {
        ke::log(__FILE__, "VA 0x%llX is not page-aligned!\r\n", virtualAddress);
        return;
    }

    else if (virtualToPhysical(virtualAddress) == 0) {
        ke::log(__FILE__, "0x%llX does not have a mapping! Can't unmap!\r\n");
        return;
    }

    uint64_t *level = kernelPml4;

    for (uint64_t i = 39; i >= 12; i -= 9) {
        PAGE_TABLE_ENTRY entry = level[((virtualAddress >> i) & 0x1FF)];

        if (i == PT_SHIFT) {
            entry = 0;
            level[((virtualAddress >> i) & 0x1FF)] = entry;
            break;
        }

        level = reinterpret_cast<uint64_t *>((entry & PT_ADDR) + hal::retrieveHhdmOffset());
    }

    __asm__ volatile ("invlpg (%0)" :: "a"(virtualAddress));
}

void hal::x64::unmapPages(uintptr_t virtualAddress, uint64_t length)
{
    if ((length & 0xFFF) != 0) {
        ke::log(__FILE__, "Length 0x%llX is not page-aligned!\r\n", length);
        return; 
    }

    for (uint64_t i = 0; i < length; i += 0x1000) {
        unmapPage(virtualAddress + i);
    }
}

uintptr_t hal::x64::virtualToPhysical(uintptr_t virtualAddress) {
    uint64_t *level = kernelPml4;

    for (uint64_t i = 39; i >= 12; i -= 9) {
        PAGE_TABLE_ENTRY entry = level[((virtualAddress >> i) & 0x1FF)];

        if (!(entry & PTE_PRESENT))
            return 0;

        if (i == PT_SHIFT)
            return (entry & PT_ADDR);

        level = reinterpret_cast<uint64_t *>((entry & PT_ADDR) + hal::retrieveHhdmOffset());
    }
    return 0;
}