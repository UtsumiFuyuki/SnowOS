/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025
 
File: ke/kernel.cpp

Description:
This file is the core source file of
the Ke module of Yuki

Author:
UtsumiFuyuki
October 28th 2025
**/

#include <typedefs.hpp>
#include <limine.h>
#include <hal/hal.hpp>
#include <hal/serial.hpp>
#include <ke/log.hpp>
#include <mm/early_alloc.hpp>
#include <hal/x64/paging.hpp>

#define YUKI_VERSION_MAJOR 0
#define YUKI_VERSION_MINOR 1
#define YUKI_VERSION_PATCH 0

// MSVC puts global constructors in a section .CRT$XCU that is ordered between .CRT$XCA and
// .CRT$XCZ.
// This is taken from managarm, thank you :3
__declspec(allocate(".CRT$XCA")) LPCVOID crt_xct = nullptr;
__declspec(allocate(".CRT$XCZ")) LPCVOID crt_xcz = nullptr;

extern "C" VOID KeRunConstructors() {
	using InitializerPtr = VOID (*)();
	UINT_PTR begin = reinterpret_cast<UINT_PTR>(&crt_xct);
	UINT_PTR end = reinterpret_cast<UINT_PTR>(&crt_xcz);
	for (UINT_PTR it = begin + sizeof(LPVOID); it < end; it += sizeof(LPVOID)) {
		auto *p = reinterpret_cast<InitializerPtr *>(it);
		(*p)();
	}
}

extern "C" VOID KeMain(LPVOID SnowBootInfo)
{
    KeRunConstructors();

    Hal::Init();
    Hal::PrintString("Snow Operating System (c) 2025, 2026 UtsumiFuyuki\n");
    Ke::Print("Yuki Kernel Version %d.%d.%d\n", YUKI_VERSION_MAJOR, YUKI_VERSION_MINOR, YUKI_VERSION_PATCH);
    Ke::Print("Booted by: ");
    
    if (SnowBootInfo == nullptr)
    {
        Ke::Print("Limine\n\n");
    }
    else
    {
        Ke::Print("SnowBoot\n");
    }

    Hal::InitCpu();
    Mm::EarlyInit();

    Hal::X64::PagingInit();
    Hal::X64::MapPage(0xCAFEB000, 0xDEADBEEF, PTE_WRITE);

    Ke::Print("Nothing more to do, halting...\r\n");
    Ke::Log(__FILE__, "Reached end of KeMain!\r\n");

    // We're done, just hang...
    Hal::HaltCpu();
}