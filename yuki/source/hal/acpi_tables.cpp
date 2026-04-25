/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/acpi.cpp

Description:
ACPI Table parsing

Author:
UtsumiFuyuki
April 25th 2026
**/

#include <cstdint>
#include <hal/hal.hpp>
#include <hal/paging.hpp>
#include <hal/acpi_tables.hpp>
#include <ke/log.hpp>
#include <ke/string.hpp>
#include <mm/mm.hpp>

XSDP *xsdp{nullptr};
XSDT *xsdt{nullptr};

bool hal::setupAcpiTables() {
    uintptr_t rsdpPhysical = hal::retrieveRsdpPhysicalAddress();

    if (rsdpPhysical == 0) {
        ke::log(__FILE__, "No RSDP detected!\r\n");
        return false;
    }
    ke::log(__FILE__, "RSDP Physical Address: 0x%llX\r\n", rsdpPhysical);

    uintptr_t rsdpPhysicalAligned = (rsdpPhysical & ~0xFFF);
    uintptr_t rsdpVirtual = mm::allocateKernelVirt(1);
    hal::mapPage(rsdpPhysicalAligned, rsdpVirtual, PAGE_WRITE);

    xsdp = reinterpret_cast<XSDP *>(rsdpVirtual + (rsdpPhysical - rsdpPhysicalAligned));

    uint64_t checksum1{};
    uint64_t checksum2{};

    for (size_t i = 0; i < sizeof(RSDP); i++) {
        checksum1 += reinterpret_cast<uint8_t *>(xsdp)[i];
    }

    for (size_t i = 0; i < sizeof(XSDP); i++) {
        checksum2 += reinterpret_cast<uint8_t *>(xsdp)[i];
    }

    if (static_cast<uint8_t>(checksum1) != 0) {
        ke::log(__FILE__, "checksum1 invalid!\r\n");
    }

    if (static_cast<uint8_t>(checksum2) != 0) {
        ke::log(__FILE__, "checksum2 invalid!\r\n");
    }

    ke::log(__FILE__, "Address of XSDT: 0x%llX\r\n", xsdp->xsdtAddress);
    ke::log(__FILE__, "Length of Table: %llu\r\n", xsdp->length);

    uintptr_t rsdtPhysicalAligned = (xsdp->xsdtAddress & ~0xFFF);
    uintptr_t rsdtVirtual = mm::allocateKernelVirt(1);
    hal::mapPage(rsdtPhysicalAligned, rsdtVirtual, PAGE_WRITE);

    xsdt = reinterpret_cast<XSDT *>(rsdtVirtual + (xsdp->xsdtAddress - rsdtPhysicalAligned));

    ke::print("ACPI Table Access initialized!\r\n");
    ke::print("ACPI Version: ");
    if (hal::retrieveAcpiVersion() == 0) {
        ke::print("1.0\r\n");
    }

    else {
        ke::print(">=2.0\r\n");
    }

    return true;
}

uint8_t hal::retrieveAcpiVersion() {
    return xsdp->rsdp.revision;
}

void *hal::acpiFindTable(const char *signature) {
    size_t entries = (xsdt->header.length - sizeof(xsdt->header)) / 8;

    for (size_t i = 0; i < entries; i++) {
        SDT_HEADER *header = reinterpret_cast<SDT_HEADER *>(xsdt->sdtPointer[i] + hal::retrieveHhdmOffset());
        if (strncmp(header->signature, signature, 4) == 0) {
            return reinterpret_cast<void *>(header);
        }
    }
    ke::log(__FILE__, "Table %s not found!\r\n", signature);
    return nullptr;
}