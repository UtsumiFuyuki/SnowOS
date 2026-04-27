/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: ldr/pe.hpp

Description:
Struct and function prototypes related to PE loading

Author:
UtsumiFuyuki
April 19th 2026
**/

#pragma once

#include <cstdint>

#define DOS_SIGNATURE 0x5a4d

typedef struct _IMAGE_FILE_DOS_HEADER
{
    uint16_t e_magic;
    uint16_t e_cblp;
    uint16_t e_cp;
    uint16_t e_crlc;
    uint16_t e_cparhdr;
    uint16_t e_minalloc;
    uint16_t e_maxalloc;
    uint16_t e_ss;
    uint16_t e_sp;
    uint16_t e_csum;
    uint16_t e_ip;
    uint16_t e_cs;
    uint16_t e_lfarlc;
    uint16_t e_ovno;
    uint16_t e_res[4];
    uint16_t e_oemid;
    uint16_t e_oeminfo;
    uint16_t e_res2[10];
    uint32_t e_lfanew;

} __attribute__((packed)) IMAGE_FILE_DOS_HEADER;

typedef struct _IMAGE_FILE_PE_HEADER
{
    uint16_t machine;
    uint16_t numberOfSections;
    uint32_t timeDateStamp;
    uint32_t pointerToSymbolTable;
    uint32_t numberOfSymbols;
    uint16_t sizeOfOptionalHeader;
    uint16_t characteristics;
} __attribute__((packed)) IMAGE_FILE_PE_HEADER;

typedef struct _IMAGE_FILE_DATA_DIRECTORY{
    uint32_t virtualAddress;
    uint32_t size;
} IMAGE_FILE_DATA_DIRECTORY;

#define IMAGE_FILE_EXECUTABLE_IMAGE 0x0002
#define OPTIONAL_HEADER_MAGIC 0x020b

// Note that this is for a PE32+
typedef struct _IMAGE_FILE_OPTIONAL_HEADERS
{
    uint16_t magic;
    uint8_t majorLinkerVersion;
    uint8_t minorLinkerVersion;
    uint32_t sizeOfCode;
    uint32_t sizeOfInitializedData;
    uint32_t sizeOfUninitializedData;
    uint32_t addressOfEntryPoint;
    uint32_t baseOfCode;
    uint64_t imageBase;
    uint32_t sectionAlignment;
    uint32_t fileAlignment;
    uint16_t majorOperatingSystemVersion;
    uint16_t minorOperatingSystemVersion;
    uint16_t majorImageVersion;
    uint16_t minorImageVersion;
    uint16_t majorSubsystemVersion;
    uint16_t minorSubsystemVersion;
    uint32_t win32VersionValue;
    uint32_t sizeOfImage;
    uint32_t sizeOfHeaders;
    uint32_t checksum;
    uint16_t subsystem;
    uint16_t dllCharacteristics;
    uint64_t sizeOfStackReserve;
    uint64_t sizeOfStackCommit;
    uint64_t sizeOfHeapReserve;
    uint64_t sizeOfHeapCommit;
    uint32_t loaderFlags;
    uint32_t numberOfRvaAndSizes;
    IMAGE_FILE_DATA_DIRECTORY dataDirectories[16];
} __attribute__((packed)) IMAGE_FILE_OPTIONAL_HEADERS;

#define IMAGE_SCN_MEM_EXECUTE 0x20000000
#define IMAGE_SCN_MEM_READ 0x40000000
#define IMAGE_SCN_MEM_WRITE 0x80000000

typedef struct _IMAGE_FILE_SECTION_HEADER
{
    char name[8];
    uint32_t virtualSize;
    uint32_t virtualAddress; // These two are relative to ImageBase
    uint32_t sizeOfRawData;
    uint32_t pointerToRawData;
    uint32_t pointerToRelocations;
    uint32_t pointerToLineNumbers;
    uint16_t numberOfRelocations;
    uint16_t numberOfLineNumbers;
    uint32_t characteristics;
} __attribute__((packed)) IMAGE_FILE_SECTION_HEADER;

#define PE_SIGNATURE 0x00004550

typedef struct _NT_HEADERS_64
{
    uint32_t signature;
    IMAGE_FILE_PE_HEADER peHeader;
    IMAGE_FILE_OPTIONAL_HEADERS optionalHeaders;
    IMAGE_FILE_SECTION_HEADER sectionTable[];
} __attribute__((packed)) NT_HEADERS_64;

namespace ldr {
    NT_HEADERS_64 *parsePe(uint8_t *buffer);
    void *loadPe(uint8_t *buffer);
}