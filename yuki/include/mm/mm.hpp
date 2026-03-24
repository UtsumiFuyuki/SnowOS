#pragma once

#include <typedefs.hpp>
#include <mm/freelist.hpp>
#include <mm/early_alloc.hpp>

typedef ULONG64 PFN_NUMBER;

typedef struct _PFN_ENTRY
{
    UINT64 Free;

    // Points to another PFNdb entry, used to build the Free List
    _PFN_ENTRY *PageEntry;
} PFN_ENTRY, *PPFN_ENTRY;

// A list of PFNdb entries
typedef struct _PAGE_LIST
{
    UINT64 PageCount;
    PPFN_ENTRY Head;
} PAGE_LIST, *PPAGE_LIST;

namespace Mm
{
    VOID Initialize();
    UINT_PTR AllocatePage();
}