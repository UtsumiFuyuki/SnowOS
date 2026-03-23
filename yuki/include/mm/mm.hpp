#pragma once

#include <typedefs.hpp>
#include <mm/freelist.hpp>
#include <mm/pfndb.hpp>
#include <mm/early_alloc.hpp>

namespace Mm
{
VOID Initialize();
VOID InitializeFreelist(limine_memmap_response *MemoryMap);
}