/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: ke/sdbg.cpp

Description:
A minimal debugging shell

Author:
UtsumiFuyuki
April 18th 2026
**/

#include "hal/paging.hpp"
#include "limine.h"
#include <cstdint>
#include <hal/hal.hpp>
#include <hal/serial.hpp>
#include <ke/string.hpp>
#include <ke/log.hpp>
#include <ke/sdbg.hpp>
#include <mm/mm.hpp>
#include <utils/math.hpp>

char inputBuffer[100]{};
size_t index{};

bool exit{false};

extern GDT gdt;

const char *memmapTypeToString(uint64_t type) {
    switch (type) {
        case (LIMINE_MEMMAP_USABLE):
            return "Usable Memory";
        case (LIMINE_MEMMAP_BAD_MEMORY):
            return "Bad Memory";
        case (LIMINE_MEMMAP_RESERVED):
            return "Reserved Memory";
        case (LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE):
            return "Bootloader Reclaimable";
        case (LIMINE_MEMMAP_ACPI_RECLAIMABLE):
            return "Acpi Reclaimable";
        case (LIMINE_MEMMAP_FRAMEBUFFER):
            return "Framebuffer";
        case (LIMINE_MEMMAP_ACPI_NVS):
            return "Acpi Nvs";
        case (LIMINE_MEMMAP_EXECUTABLE_AND_MODULES):
            return "Executable and Modules";
        default:
            return "Unrecognized";
    }
}

const char *commandStrings[5] {
    "!help",
    "!gdt",
    "!regdump",
    "!pfn",
    "!memmap",
};

void com_help(void *) {
    ke::print("List of commands:\r\n!gdt - Displays the Global Descriptor Table\r\n!regdump - Dumps contents of the registers\r\n");
    ke::print("!pfn - Displays info about an entry in the PFNdb\r\n!memmap - Displays memory map\r\n");
}

void com_gdt(void *) {
    ke::print("0x%llX\r\n", gdt.nullSegment);
    ke::print("0x%llX\r\n", gdt.kernelCode);
    ke::print("0x%llX\r\n", gdt.kernelData);
    ke::print("0x%llX\r\n", gdt.userCode);
    ke::print("0x%llX\r\n", gdt.userData);
}

void com_regdump(void *) {
    uint64_t rax{};
    uint64_t rbx{};
    uint64_t rcx{};
    uint64_t rdx{};
    uint64_t rdi{};
    uint64_t rsi{};

    __asm__ volatile ("" : "=a"(rax), "=b"(rbx), "=c"(rcx), "=d"(rdx), "=D"(rdi), "=S"(rsi));

    ke::print("RAX: 0x%llX RBX: 0x%llX RCX: 0x%llX RDX: 0x%llX RDI: 0x%llX RSI: 0x%llX\r\n", rax, rbx, rcx, rdx, rdi, rsi);
}

void com_pfn(void *args) {
    uint64_t pfn = reinterpret_cast<uint64_t>(args);

    ke::print("===== PFN INFO =====\r\n");

    PFN_ENTRY entry = mm::pfnInfo(pfn);
    ke::print("PFN Number: %llu\r\n", pfn);
    ke::print("Free: %s\r\n", entry.free ? "yes" : "no");
}

void com_memmap(void *) {
    ke::print("===== MEMORY MAP =====\r\n");
    limine_memmap_response *memoryMap = hal::retrieveMemoryMap();

    for (size_t i = 0; i < memoryMap->entry_count; i++) {
        limine_memmap_entry *entry = memoryMap->entries[i];
        ke::print("Type: %s\r\nBase: 0x%llX\r\nLength: 0x%llX\r\n", memmapTypeToString(entry->type), entry->base, entry->length);
    }
}

typedef void(*commandFunc)(void*);
commandFunc commands[5] {
    com_help,
    com_gdt,
    com_regdump,
    com_pfn,
    com_memmap
};



// TODO: Add arch abstractions
int parseCommand(const char *command) {
    // TODO: use strcmp instead
    for (size_t i = 0; i < 5; i++) {
        if (memcmp(command, commandStrings[i], strlen(commandStrings[i])) == 0) {
            if (i == 3)
                commands[i](reinterpret_cast<void *>(atoi(command + 5)));
            else
                commands[i](nullptr);
            return 0;
        }
    }
    return -1;
}

void keBeginDebugSession() {
    ke::print("SnowOS Kernel Debugger session started!\r\n\r\n");
    while (!exit) {
        ke::print("sdbg>");
        for (;;) {
            char c = hal::readSerial(COM1);
            if (c != '\0') {
                inputBuffer[index] = c;
                index++;
            }
            ke::print("%c", c);

            if (c == '\n' || c == '\r') {
                ke::print("\r\n");

                if(parseCommand(inputBuffer) != 0) {
                    ke::print(ANSI_RED "Unrecognized command!\r\n" ANSI_RESET);
                }

                memset(inputBuffer, 0, 100);
                index = 0;
                break;
            }
        }
    }
}