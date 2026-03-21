#include <typedefs.hpp>

#define PML4_SHIFT 39
#define PDPT_SHIFT 30
#define PD_SHIFT 21
#define PT_SHIFT 12

#define PML4_ID(virt) (((virt) >> PML4_SHIFT) & 0x1FF)
#define PDPT_ID(virt) (((virt) >> PDPT_SHIFT) & 0x1FF)
#define PD_ID(virt) (((virt) >> PD_SHIFT) & 0x1FF)
#define PT_ID(virt) (((virt) >> PT_SHIFT) & 0x1FF)

#define PT_ADDR 0x0000FFFFFFFFF000

#define PTE_PRESENT (1ULL << 0)
#define PTE_WRITE (1ULL << 1)
#define PTE_USER (1ULL << 2)
#define PTE_PWT (1ULL << 3)
#define PTE_PCD (1ULL << 4)

#define PTE_EXECUTE_DISABLE (1ULL << 63)

typedef UINT64 PAGE_TABLE_ENTRY;

namespace Hal
{
    namespace X64
    {
        VOID PagingInit();
        VOID SetCr3(UINT_PTR Pml4);
        PAGE_TABLE_ENTRY CreateNewEntry(UINT64 Flags);
        VOID MapPage(UINT_PTR PhysicalAddress, UINT_PTR VirtualAddress, UINT64 Flags);
        VOID MapPages(UINT_PTR PhysicalAddress, UINT_PTR VirtualAddress, UINT64 Length, UINT64 Flags);
    }
}