/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025
 
File: io/gz.cpp

Description:
GZ decompression stuff

Uses KGZ (KernelGZ) provided by Evalyn and Luna

Author:
UtsumiFuyuki
April 23rd 2026
**/

#include <cstdint>
#include <hal/hal.hpp>
#include <mm/slab.hpp>
#include <ke/log.hpp>
#include <ke/string.hpp>
#include <io/gz.hpp>

#define KGZ_USE_OWN_MACROS
#define KGZ_MALLOC(size) mm::allocatePool((size))
#define KGZ_FREE(ptr, size) mm::freePool((ptr))
#define KGZ_MEMCPY(dst, src, n) memcpy((dst), (src), (n))
#define KGZ_MEMSET(ptr, val, size) memset((ptr), (val), (size))
// this may become a NOP
#define KGZ_PRINTF(...) ke::print(__VA_ARGS__)

// continue to use
#define KGZ_IMPLEMENTATION
#include <io/kgz_singleheader.hpp>

typedef struct _TAR_FILE_HEADER {
    char name[100];
    uint64_t fileMode;
    uint64_t ownerUID;
    uint64_t groupUID;
    char fileSize[12];
    char modificationTime[12];
    uint64_t checksum;
    char linkIndicator;
    char linkedName[100];
    char ustar[6];
    char ustarVersion[2];
    char ownerName[32];
    char groupName[32];
    uint64_t deviceMinor;
    uint64_t deviceMajor;
    char filenamePrefix[155];
} __attribute__((packed)) TAR_FILE_HEADER;

int oct2bin(unsigned char *str, int size) {
    int n = 0;
    unsigned char *c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}

void io::parseModules() {
    limine_module_response *modules = hal::retrieveModules();

    ke::print("Number of modules detected: %llu\r\n", modules->module_count);

    limine_file *initrd = modules->modules[0];
    void * filePtr = initrd->address;
    size_t fileSize = initrd->size;

    uint64_t resultSize{};
    uint64_t bufferSize{};

    void *result = kgz_gzip_decompress(filePtr, fileSize, &resultSize, &bufferSize);

    if (result == nullptr) {
        ke::print("big oof\r\n");
    }

    TAR_FILE_HEADER *fileHdr = reinterpret_cast<TAR_FILE_HEADER *>(result);

    ke::print("%s\r\n", fileHdr->ustar);
}