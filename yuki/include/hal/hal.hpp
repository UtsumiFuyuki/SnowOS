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

#include <typedefs.hpp>
#include <limine.h>

#define KERNEL_CS 0x00af9b000000ffff
#define KERNEL_DS 0x00af93000000ffff

#define USER_CS 0x00affb000000ffff
#define USER_DS 0x00aff3000000ffff

typedef struct _DTR
{
    UINT16 Limit;
    UINT64 Base;
} __attribute__((packed)) DTR; // Descriptor Table Register

typedef struct _GDT
{
    UINT64 NullSegment;
    UINT64 KernelCode;
    UINT64 KernelData;
    UINT64 UserCode;
    UINT64 UserData;
} __attribute__((packed)) GDT;

typedef struct _IDT_ENTRY
{
    UINT16 IsrLow;
    UINT16 SegmentSelector;
    UINT8 Ist;
    UINT8 Attributes;
    UINT16 IsrMid;
    UINT32 IsrHigh;
    UINT32 Reserved;
} __attribute__((packed)) IDT_ENTRY;

namespace Hal
{
    VOID Init();
    VOID PrintString(LPCSTR String);
    VOID HaltCpu();
    VOID InitCpu();

    // Starts up the other CPUs in the system
    VOID InitSmp();

    UINT64 RetrieveHhdmOffset();
    limine_memmap_response *RetrieveMemoryMap();
    UINT_PTR RetrieveRsdpPhysicalAddress();

    LPCSTR BlVersion();
}

extern "C" BOOL HalInterruptsEnabled();