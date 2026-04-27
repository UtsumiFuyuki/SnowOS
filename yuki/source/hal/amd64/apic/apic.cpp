/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/apic/lapic.cpp

Description:
Functions for configuring the Local APIC

Author:
UtsumiFuyuki
April 25th 2026
**/

#include <cstdint>
#include <hal/amd64/msr.hpp>
#include <hal/amd64/cpuid.hpp>
#include <hal/amd64/timers/hpet.hpp>
#include <hal/amd64/apic/apic.hpp>
#include <hal/paging.hpp>
#include <ke/log.hpp>
#include <mm/mm.hpp>
#include <utils/mmio.hpp>

extern "C" uintptr_t apicMmioBase{};

void hal::x64::enableLapic() {
    uint64_t apicBaseMsr = rdmsr(IA32_APIC_BASE);
    ke::log(__FILE__, "Base address of LAPIC: 0x%llX\r\n", apicBaseMsr & ~0xFFF);

    apicMmioBase = mm::allocateKernelVirt(2);
    hal::mapPages((apicBaseMsr & ~0xFFF), apicMmioBase, 0x2000, PAGE_WRITE | PAGE_NO_EXECUTE | PAGE_CACHE_DISABLE | PAGE_WRITE_THROUGH);

    if (((apicBaseMsr >> 11) & 1) != 1) {
        ke::log(__FILE__, "APIC is not enabled!\r\n");
        apicBaseMsr |= APIC_ENABLED;
        wrmsr(IA32_APIC_BASE, apicBaseMsr);
    }
    else
        ke::log(__FILE__, "APIC is enabled!\r\n");

    uint32_t spiv{0x1FF};
    mmioWrite32(reinterpret_cast<uint64_t *>(apicMmioBase + LAPIC_SPURIOUS_IVT_REG), spiv);

    mmioWrite32(reinterpret_cast<uint64_t *>(apicMmioBase + LAPIC_DCR), 0x3);
    mmioWrite32(reinterpret_cast<uint64_t *>(apicMmioBase + LAPIC_LVT_ICOUNT), 0xFFFFFFFF);
    hal::x64::hpetSleep(10);
    mmioWrite32(reinterpret_cast<uint64_t *>(apicMmioBase + LAPIC_LVT_TMR), APIC_MASKED);
    uint32_t ticks = 0xFFFFFFFF - mmioRead32(reinterpret_cast<uint64_t *>(apicMmioBase + LAPIC_LVT_CCOUNT));
    ke::log(__FILE__, "LAPIC ticked %llu times in 10ms\r\n", ticks);
    mmioWrite32(reinterpret_cast<uint64_t *>(apicMmioBase + LAPIC_DCR), 0x3);
    mmioWrite32(reinterpret_cast<uint64_t *>(apicMmioBase + LAPIC_LVT_TMR), 0x2F | APIC_PERIODIC_MODE);
    mmioWrite32(reinterpret_cast<uint64_t *>(apicMmioBase + LAPIC_LVT_ICOUNT), ticks);

    // Enable recieving timer interrupts
    mmioWrite32(reinterpret_cast<uint64_t *>(apicMmioBase + LAPIC_EOI_REG), 0);
}