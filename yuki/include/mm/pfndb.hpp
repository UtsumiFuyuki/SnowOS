#pragma once

#include <typedefs.hpp>

typedef ULONG64 PFN_NUMBER;

typedef struct _PFN_ENTRY
{
    UINT64 Free;
} PFN_ENTRY, *PPFN_ENTRY;