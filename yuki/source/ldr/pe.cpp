/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: ldr/pe.cpp

Description:
PE Loader (except not really, there's only parsing here right now =P)

Author:
UtsumiFuyuki
April 19th 2026
**/

#include <cstdint>
#include <hal/paging.hpp>
#include <ke/log.hpp>
#include <ke/string.hpp>
#include <mm/mm.hpp>
#include <ldr/pe.hpp>

NT_HEADERS_64 *ldr::parsePe(uint8_t *buffer) {
    NT_HEADERS_64 *header{};
    IMAGE_FILE_DOS_HEADER *dosHeader = reinterpret_cast<IMAGE_FILE_DOS_HEADER *>(buffer);

    if (dosHeader->e_magic != DOS_SIGNATURE) {
        ke::print(ANSI_RED "PE has an invalid DOS_SIGNATURE!\r\n" ANSI_RESET);
        return nullptr;
    }

    header = reinterpret_cast<NT_HEADERS_64 *>(buffer + dosHeader->e_lfanew);
    if (header->signature != PE_SIGNATURE) {
        ke::print(ANSI_RED "PE has an invalid PE_SIGNATURE!\r\n" ANSI_RESET);
        return nullptr;
    }

    return header;
}

void *ldr::loadPe(uint8_t *buffer) {
    NT_HEADERS_64 *header = ldr::parsePe(buffer);
    
    if (header == nullptr) {
        ke::print(ANSI_RED "Failed to load PE!\r\n" ANSI_RESET);
        return nullptr;
    }

    uintptr_t imageBase = header->optionalHeaders.imageBase;

    ke::log(__FILE__, "Size of image: %llu | 0x%llX\r\n", header->optionalHeaders.sizeOfImage, header->optionalHeaders.sizeOfImage);

    void *image = mm::allocateUserPages(header->optionalHeaders.sizeOfImage / 0x1000);

    for (size_t i = 0; i < header->peHeader.numberOfSections; i++) {
        ke::log(__FILE__, "raw data: 0x%llX\r\n", (header->sectionTable[i].pointerToRawData));
        ke::log(__FILE__, "virtual size: 0x%llX\r\n", (header->sectionTable[i].virtualSize));

        memcpy(reinterpret_cast<void *>((uintptr_t)image + (header->sectionTable[i].virtualAddress)), (buffer + header->sectionTable[i].pointerToRawData), header->sectionTable[i].sizeOfRawData);
    }

    return reinterpret_cast<void *>((uintptr_t)image + header->optionalHeaders.addressOfEntryPoint);
}