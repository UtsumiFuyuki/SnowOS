/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/apic/lapic.hpp

Description:
Structs and Prototypes related to the Local APIC

Author:
UtsumiFuyuki
April 25th 2026
**/

#pragma once

#define LAPIC_ID_REG            0x20
#define LAPIC_VERSION_REG       0x30
#define LAPIC_TPR               0x80
#define LAPIC_APR               0x90
#define LAPIC_PPR               0xA0
#define LAPIC_EOI_REG           0xB0
#define LAPIC_RRD               0xC0
#define LAPIC_LDR               0xD0
#define LAPIC_DFR               0xE0
#define LAPIC_SPURIOUS_IVT_REG  0xF0
#define LAPIC_ISR               0x100
#define LAPIC_TMR               0x180
#define LAPIC_IRR               0x200
#define LAPIC_ESR               0x280
#define LAPIC_CMCI_REG          0x2F0
#define LAPIC_ICR               0x300
#define LAPIC_LVT_TMR           0x320
#define LAPIC_LVT_TSR           0x330
#define LAPIC_LVT_PMCR          0x340
#define LAPIC_LVT_LINT0         0x350
#define LAPIC_LVT_LINT1         0x360
#define LAPIC_LVT_ERROR_REG     0x370
#define LAPIC_LVT_ICOUNT        0x380
#define LAPIC_LVT_CCOUNT        0x390
#define LAPIC_DCR               0x3E0

#define APIC_ENABLED (1ULL << 11)

#define APIC_PERIODIC_MODE (1ULL << 17)
#define APIC_TSC_DEADLINE_MODE (1ULL << 18)
#define APIC_MASKED (1ULL << 16)

namespace hal {
    namespace x64 {
        void enableLapic();
    }
}