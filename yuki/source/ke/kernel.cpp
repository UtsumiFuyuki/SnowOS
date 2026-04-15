/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: ke/kernel.cpp

Description:
This file is the core source file of
the Ke module of Yuki

Author:
UtsumiFuyuki
October 28th 2025
**/

#include <hal/hal.hpp>
#include <hal/paging.hpp>
#include <hal/serial.hpp>
#include <hal/amd64/cpuid.hpp>
#include <ke/log.hpp>
#include <ke/string.hpp>
#include <mm/mm.hpp>
#include <uacpi/uacpi.h>

#define YUKI_VERSION_MAJOR 0
#define YUKI_VERSION_MINOR 1
#define YUKI_VERSION_PATCH 0

// MSVC puts global constructors in a section .CRT$XCU that is ordered between .CRT$XCA and
// .CRT$XCZ.
// This is taken from managarm, thank you :3
__declspec(allocate(".CRT$XCA")) const void *crt_xct = nullptr;
__declspec(allocate(".CRT$XCZ")) const void *crt_xcz = nullptr;

extern "C" void keRunConstructors() {
	using InitializerPtr = void (*)();
	uintptr_t begin = reinterpret_cast<uintptr_t>(&crt_xct);
	uintptr_t end = reinterpret_cast<uintptr_t>(&crt_xcz);
	for (uintptr_t it = begin + sizeof(void *); it < end; it += sizeof(void *)) {
		auto *p = reinterpret_cast<InitializerPtr *>(it);
		(*p)();
	}
}

extern "C" void keMain(void *snowbootInfo) {
    keRunConstructors();

    hal::initialize();
    hal::printString("Snow Operating System (c) 2025, 2026 UtsumiFuyuki\r\n");
    ke::print("Yuki Kernel Version %d.%d.%d\r\n", YUKI_VERSION_MAJOR, YUKI_VERSION_MINOR, YUKI_VERSION_PATCH);
    ke::print("Booted by: ");
    
    if (snowbootInfo == nullptr) {
        ke::print("Limine %s\r\n\r\n", hal::blVersion());
    }
    else {
        ke::print("SnowBoot\r\n");
    }

    hal::initCpu();

    CPUID_REGISTERS cpuid = hal::x64::getCpuid(0, 0);
    char vendorId[13];
    vendorId[12] = '\0';

    memcpy(vendorId, &cpuid.rbx, 4);
    memcpy(vendorId + 4, &cpuid.rdx, 4);
    memcpy(vendorId + 8, &cpuid.rcx, 4);

    ke::print("Vendor ID: %s\r\n", vendorId);

    uint64_t *test = reinterpret_cast<uint64_t *>(0x1000);
    *test = 12;

    mm::earlyInit();
    hal::initializePaging();
    mm::initialize();
    mm::initializeVmm();

    // We're done, just hang...
    hal::haltCpu();
}