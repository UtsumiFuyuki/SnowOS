/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/timers/hpet.hpp

Description:
Structs and Prototypes related to the High Precision Event Timer (HPET)

Author:
UtsumiFuyuki
April 25th 2026
**/

#pragma once

#include <hal/acpi_tables.hpp>
#include <cstdint>

#define HPET_GENERAL_CAPABALITIES       0x0
#define HPET_GENERAL_CONFIGURATION      0x10
#define HPET_GENERAL_INTERRRUPT_STATUS  0x20
#define HPET_MAIN_COUNTER_VALUE         0xF0
#define HPET_TIMER0_CCR                 0x100
#define HPET_TIMER0_CVR                 0x108
#define HPET_TIMER0_FIRR                0x110
#define HPET_TIMER1_CCR                 0x120
#define HPET_TIMER1_CVR                 0x128
#define HPET_TIMER1_FIRR                0x130
#define HPET_TIMER2_CCR                 0x140
#define HPET_TIMER2_CVR                 0x148
#define HPET_TIMER2_FIRR                0x150

// General Capabilties Fields
#define HPET_REV_ID                     0xFF
#define HPET_COUNTER_CLOCK_PERIOD       0xFFFFFFFF

#define HPET_ENABLE_CNF 0x1

typedef struct _HPET {
    SDT_HEADER header;
    uint32_t eventTimerBlockId;
    uint8_t addressSpaceId; // 0 - System Memory | 1 - System IO
    uint8_t registerBitWidth;
    uint8_t registerBitOffset;
    uint8_t reserved;
    uint64_t baseAddress;
    uint8_t hpetNumber;
    uint16_t minimumClockTick;
    uint8_t attributes;
} __attribute__((packed)) HPET;

namespace hal {
    namespace x64 {
        void enableHpet();
        void hpetSleep(uint64_t ms);
    }
}