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

#include <ke/log.hpp>
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