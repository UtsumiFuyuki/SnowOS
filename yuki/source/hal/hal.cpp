/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025
 
File: hal/hal.cpp

Description:
This file is the main source file of
Hardware Abstraction Layer module of Yuki

Author:
UtsumiFuyuki
October 28th 2025
**/

#include <typedefs.hpp>
#include <cstddef>
#include <limine.h>
#include <flanterm.h>
#include <flanterm_backends/fb.h>
#include <hal/hal.hpp>
#include <hal/serial.hpp>
#include <ke/string.hpp>
#include <ke/log.hpp>

// Limine Stuff

// Set the base revision to 4, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

namespace {

__attribute__((used, section(".limine_requests")))
volatile LIMINE_BASE_REVISION(4);

}

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

namespace {

__attribute__((used, section(".limine_requests")))
volatile limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile limine_memmap_request LimineMemoryMapRequest = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile limine_hhdm_request LimineHhdmRequest = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = nullptr
};

}

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .cpp file, as seen fit.

namespace {

__attribute__((used, section(".limine_requests_start")))
volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
volatile LIMINE_REQUESTS_END_MARKER;

}

struct flanterm_context* FtCtx;

UINT32 TerminalForeground = 0xFFFFFF;
UINT32 TerminalBackground = 0x0000AD;

extern "C" VOID ReloadSegments();
extern "C" LPVOID IsrStubTable[];

GDT Gdt {0, KERNEL_CS, KERNEL_DS, USER_CS, USER_DS};
DTR GdtRegister;

IDT_ENTRY Idt[256];
DTR Idtr;

limine_framebuffer *Framebuffer;

// The following stubs are required by the Itanium C++ ABI (the one we use,
// regardless of the "Itanium" nomenclature).
// Like the memory functions above, these stubs can be moved to a different .cpp file,
// but should not be removed, unless you know what you are doing.
extern "C" {
    INT __cxa_atexit(VOID (*)(LPVOID), LPVOID, LPVOID) { return 0; }
    VOID __cxa_pure_virtual() { Hal::HaltCpu(); }
    VOID *__dso_handle;
}

VOID HalIdtSetDescriptor(UINT8 Vector, LPVOID Isr, UINT8 Flags) {
    IDT_ENTRY* Descriptor = &Idt[Vector];

    Descriptor->IsrLow              = (UINT64)Isr & 0xFFFF;
    Descriptor->SegmentSelector     = 0x08;
    Descriptor->Ist                 = 0;
    Descriptor->Attributes          = Flags;
    Descriptor->IsrMid              = ((UINT64)Isr >> 16) & 0xFFFF;
    Descriptor->IsrHigh             = ((UINT64)Isr >> 32) & 0xFFFFFFFF;
    Descriptor->Reserved            = 0;
}

VOID Hal::Init()
{
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        Hal::HaltCpu();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == nullptr
     || framebuffer_request.response->framebuffer_count < 1) {
        Hal::HaltCpu();
    }

    // Fetch the first framebuffer.
    Framebuffer = framebuffer_request.response->framebuffers[0];

    FtCtx = flanterm_fb_init(
        NULL,
        NULL,
        reinterpret_cast<UINT32 *>(Framebuffer->address), Framebuffer->width, Framebuffer->height, Framebuffer->pitch,
        Framebuffer->red_mask_size, Framebuffer->red_mask_shift,
        Framebuffer->green_mask_size, Framebuffer->green_mask_shift,
        Framebuffer->blue_mask_size, Framebuffer->blue_mask_shift,
        NULL,
        NULL, NULL,
        &TerminalBackground, &TerminalForeground,
        NULL, NULL,
        NULL, 0, 0, 1,
        0, 0,
        0
    );

    Hal::InitializeSerial(COM1);
}

VOID Hal::PrintString(LPCSTR String)
{
    flanterm_write(FtCtx, String, strlen(String));
    Hal::WriteStringToSerial(COM1, String);
}

VOID Hal::HaltCpu()
{
    for (;;)
    {
        __asm__ volatile ("hlt");
    }
}

VOID Hal::InitCpu()
{
    // Setup the GDT
    GdtRegister.Base = reinterpret_cast<UINT_PTR>(&Gdt);
    GdtRegister.Limit = (sizeof(Gdt) - 1);

    __asm__ volatile ("lgdt %0" :: "m"(GdtRegister));
    ReloadSegments();

    // Setup the IDT
    Idtr.Base = (UINT_PTR)&Idt;
    Idtr.Limit = (UINT16)sizeof(IDT_ENTRY) * 256 - 1;

    for(INT i = 0; i < 40; i++)
    {
        HalIdtSetDescriptor(i, IsrStubTable[i], 0x8e);
    }

    __asm__ volatile ("lidt %0" :: "m"(Idtr));
    Ke::Print("Cpu Initialized!\n");
}

UINT64 Hal::RetrieveHhdmOffset()
{
    return LimineHhdmRequest.response->offset;
}

limine_memmap_response *Hal::RetrieveMemoryMap()
{
    limine_memmap_response *MemoryMap = LimineMemoryMapRequest.response;
    return MemoryMap;
}