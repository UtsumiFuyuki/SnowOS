/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/acpi.hpp

Description:
Prototypes for ACPI table parsing

Author:
UtsumiFuyuki
April 25th 2026
**/

#pragma once

#include <cstddef>
#include <cstdint>

typedef struct _RSDP {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdtAddress;
} __attribute__((packed)) RSDP;

typedef struct _XSDP {
    RSDP rsdp;

    uint32_t length;
    uint64_t xsdtAddress;
    uint8_t extendedChecksum;
    uint8_t reserved[8];
} __attribute__((packed)) XSDP;

static_assert(offsetof(XSDP, rsdp) == 0, "Incorrect offset for rsdp in XSDP!");
static_assert(offsetof(XSDP, length) == 20, "Incorrect offset for extended fields in XSDP!");

typedef struct _SDT_HEADER {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oemTableId[8];
    uint32_t oemRevision;
    uint32_t creatorid;
    uint32_t createrRevision;
} __attribute__((packed)) SDT_HEADER;

typedef struct _XSDT {
    SDT_HEADER header;
    uintptr_t sdtPointer[] __attribute__((packed, aligned(4)));
} XSDT;

namespace hal {
    bool setupAcpiTables();
    uint8_t retrieveAcpiVersion();
    void *acpiFindTable(const char *signature);
}