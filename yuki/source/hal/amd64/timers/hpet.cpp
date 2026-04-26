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

#define HPET_COUNTER_CLK_PERIOD_MAX 0x05F5E100

uintptr_t hpetMmioBase{};
uint64_t hpetFrequency{};

uint64_t fsToNs(uint64_t fs) {
    return (fs / 1000000);
}

uint64_t nsTofs(uint64_t ns) {
    return (ns * 1000000);
}

uint64_t readHpetRegister(uint64_t reg) {
    return *reinterpret_cast<uint64_t *>(hpetMmioBase + reg);
}

void writeHpetRegister(uint64_t value, uint64_t reg) {
    *reinterpret_cast<uint64_t *>(hpetMmioBase + reg) = value;
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
    hal::mapPage(hpet->baseAddress, hpetMmioBase, PAGE_WRITE);
    uint32_t counterClkPeriod = static_cast<uint32_t>(readHpetRegister(HPET_GENERAL_CAPABALITIES) >> 32);

    if (counterClkPeriod > HPET_COUNTER_CLK_PERIOD_MAX) {
        ke::log(__FILE__, "COUNTER_CLK_PERIOD_MAX is greater than allowed!\r\n");
    }

    hpetFrequency = 1000000000000000 / counterClkPeriod;
    ke::print("Frequency of HPET is: %lluMHz\r\n", (hpetFrequency / 1000000));

    // Set ENABLE_CNF
    writeHpetRegister(readHpetRegister(HPET_GENERAL_CONFIGURATION) | HPET_ENABLE_CNF, HPET_GENERAL_CONFIGURATION);
    ke::print("HPET Initialized!\r\n");
}

void hal::x64::hpetSleep(uint64_t ms) {
    const uint64_t ticks = (ms * hpetFrequency) / 1'000;
    uint64_t startCount = readHpetRegister(HPET_MAIN_COUNTER_VALUE);

    while ((readHpetRegister(HPET_MAIN_COUNTER_VALUE) - startCount) < ticks);
}