/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/slab.hpp

Description:
Structs and function declarations for a Slab implementation

Author:
UtsumiFuyuki
April 15th, 2026
**/

#pragma once

#include <cstdint>

typedef struct _SLAB {

    typedef struct _BUFCTL_SMALL {
        _BUFCTL_SMALL *next;
    } BUFCTL_SMALL;

    // External Bufctl
    typedef struct _BUFCTL {
        _SLAB * slab; // Pointer to the slab of which this bufctl belongs to
        _BUFCTL *next;
    } BUFCTL;

    _SLAB *prev;
    size_t objectSize;
    uint64_t refCount;

    union {
        _BUFCTL_SMALL *bufctlSmall;
        _BUFCTL *bufctl;
    };

    _SLAB *next;
} SLAB;

typedef struct _SLAB_CACHE {
    size_t cacheSize;
    SLAB *list;
    SLAB *freelist;
} SLAB_CACHE;

namespace mm {
    void initializeSlab();

    SLAB_CACHE *createCache(size_t objectSize);
    SLAB *createSlabSmall(size_t objectSize);

    void displayList(size_t n);

    void *allocatePool(size_t numberOfBytes);
    void freePool(void *address);
};