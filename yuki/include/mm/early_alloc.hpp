/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: mm/early_alloc.hpp

Description:
This file contains functions declarations for the
bootstrap allocator

Author:
UtsumiFuyuki
March 19th 2026
**/

#include <typedefs.hpp>

typedef struct _EARLY_ALLOC_REGION
{
    UINT_PTR Base;
    UINT64 Size;
} EARLY_ALLOC_REGION;

namespace Mm
{
    VOID EarlyInit();
    UINT_PTR EarlyAllocatePage();
}