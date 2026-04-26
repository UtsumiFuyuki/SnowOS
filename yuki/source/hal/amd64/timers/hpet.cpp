/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/timers/hpet.cpp

Description:
Functions for configuring the High Precision Event Timer

Author:
UtsumiFuyuki
April 25th 2026
**/

#include <cstdint>
#include <hal/hal.hpp>
#include <hal/paging.hpp>
#include <hal/acpi_tables.hpp>
#include <hal/amd64/timers/hpet.hpp>
#include <ke/log.hpp>
#include <mm/mm.hpp>
#include <utils/mmio.hpp>

#define HPET_COUNTER_CLK_PERIOD_MAX 0x05F5E100

uintptr_t hpetMmioBase{};
uint64_t hpetFrequency{};

uint64_t fsToNs(uint64_t fs) {
    return (fs / 1000000);
}

void hal::x64::enableHpet() {
    HPET *hpet = reinterpret_cast<HPET *>(hal::acpiFindTable("HPET"));

    if (hpet == nullptr) {
        ke::print(ANSI_RED "HPET does not exist on machine!\r\n");
        // TODO: fall back to other timers
        hal::haltCpu();
    }

    ke::log(__FILE__, "Base address of HPET: 0x%llX\r\n", hpet->baseAddress);

    hpetMmioBase = mm::allocateKernelVirt(1);
    hal::mapPage(hpet->baseAddress, hpetMmioBase, PAGE_WRITE | PAGE_NO_EXECUTE | PAGE_CACHE_DISABLE | PAGE_WRITE_THROUGH);
    uint32_t counterClkPeriod = static_cast<uint32_t>(mmioRead64(reinterpret_cast<uint64_t *>(hpetMmioBase + HPET_GENERAL_CAPABALITIES)) >> 32);

    ke::log(__FILE__, "COUNTER_CLOCK_PERIOD: 0x%llX\r\n", counterClkPeriod);

    if (counterClkPeriod > HPET_COUNTER_CLK_PERIOD_MAX) {
        ke::log(__FILE__, "COUNTER_CLK_PERIOD_MAX is greater than allowed!\r\n");
    }

    hpetFrequency = 1000000000000000 / counterClkPeriod;
    ke::print("Frequency of HPET is: %lluMHz\r\n", (hpetFrequency / 1000000));

    mmioWrite64(reinterpret_cast<uint64_t *>(hpetMmioBase + HPET_GENERAL_CONFIGURATION), mmioRead64(reinterpret_cast<uint64_t *>(hpetMmioBase + HPET_GENERAL_CONFIGURATION)) & ~HPET_ENABLE_CNF);
    mmioWrite64(reinterpret_cast<uint64_t *>(hpetMmioBase + HPET_GENERAL_CONFIGURATION), 0);
    mmioWrite64(reinterpret_cast<uint64_t *>(hpetMmioBase + HPET_GENERAL_CONFIGURATION), mmioRead64(reinterpret_cast<uint64_t *>(hpetMmioBase + HPET_GENERAL_CONFIGURATION)) | HPET_ENABLE_CNF);
    ke::print("HPET Initialized!\r\n");
}

void hal::x64::hpetSleep(uint64_t ms) {
    const uint64_t ticks = (ms * hpetFrequency) / 1'000;
    uint64_t startCount = mmioRead64(reinterpret_cast<uint64_t *>(hpetMmioBase + HPET_MAIN_COUNTER_VALUE));

    while ((mmioRead64(reinterpret_cast<uint64_t *>(hpetMmioBase + HPET_MAIN_COUNTER_VALUE)) - startCount) < ticks);
}