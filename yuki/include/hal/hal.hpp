/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025
 
File: hal/hal.hpp

Description:
This file contain architecture specific
structures and definitions for Hal

Author:
UtsumiFuyuki
October 28th 2025
**/

#pragma once

#include <cstdint>
#include <limine.h>

#define KERNEL_CS 0x00af9b000000ffff
#define KERNEL_DS 0x00af93000000ffff

#define USER_CS 0x00affb000000ffff
#define USER_DS 0x00aff3000000ffff

typedef struct _DTR {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) DTR; // Descriptor Table Register

typedef struct _TSS {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist[7];
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iopb;
} __attribute__((packed)) TSS;

typedef struct _TSS_DESC {
    uint16_t limit1;
    uint16_t base1;
    uint8_t base2;
    uint8_t accessByte;
    uint8_t flags;
    uint8_t base3;
    uint32_t base4;
    uint32_t reserved;
} __attribute__((packed)) TSS_DESC;

typedef struct _GDT {
    uint64_t nullSegment;
    uint64_t kernelCode;
    uint64_t kernelData;
    uint64_t userData;
    uint64_t userCode;
    TSS_DESC tssDescriptor;
} __attribute__((packed)) GDT;

typedef struct _IDT_ENTRY {
    uint16_t isrLow;
    uint16_t segmentSelector;
    uint8_t ist;
    uint8_t attributes;
    uint16_t isrMid;
    uint32_t isrHigh;
    uint32_t reserved;
} __attribute__((packed)) IDT_ENTRY;

namespace hal {
    void initialize();
    void printString(const char *String);
    void haltCpu();
    void initCpu();

    void setRsp0(uintptr_t rsp0);

    // Starts up the other CPUs in the system
    void initSmp();

    void setupSyscall();

    uint64_t retrieveHhdmOffset();
    limine_memmap_response *retrieveMemoryMap();
    uintptr_t retrieveRsdpPhysicalAddress();
    
    // Returns the executable image of yuki
    limine_file *retrieveYukiImage();

    uintptr_t yukiPhysicalAddress();
    uintptr_t yukiVirtualAddress();

    limine_module_response *retrieveModules();

    const char *blVersion();
}

extern "C" bool halInterruptsEnabled();