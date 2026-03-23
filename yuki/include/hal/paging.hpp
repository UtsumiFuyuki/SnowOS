#pragma once

#include <typedefs.hpp>

#define PAGE_WRITE 0x1
#define PAGE_USER 0x2
#define PAGE_NO_EXECUTE 0x4

namespace Hal
{
    VOID InitializePaging();
    VOID MapPage(UINT_PTR PhysicalAddress, UINT_PTR VirtualAddress, UINT_PTR Flags);
    VOID MapPages(UINT_PTR PhysicalAddress, UINT_PTR VirtualAddress, UINT64 Length, UINT64 Flags);
    UINT_PTR VirtualToPhysical(UINT_PTR VirtualAddress);
    UINT64 GeneralFlagsToArchSpecific(UINT64 Flags);
}