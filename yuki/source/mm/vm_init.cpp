#include <mm/mm.hpp>
#include <mm/vmem.hpp>
#include <ke/log.hpp>

// Bootstrao Heap Arena
VMEM_ARENA HeapArena{};

// Bootstrap page to put boundry tags
PVMEM_BOUNDARY_TAG InitBtPage{};

VOID Mm::InitializeVmm()
{
    Ke::Print("Initializing Virtual Memory Manager...\r\n");
    Mm::VmemCreateArena(&HeapArena, "HeapArena", reinterpret_cast<PVOID>(0x1000), 0x10000, 0x1000, nullptr, nullptr, nullptr);
    Ke::Log(__FILE__, "Name: %s Base: 0x%llX Size: 0x%llX Quantum: 0x%llX\r\n", HeapArena.Name, HeapArena.Base, HeapArena.Size, HeapArena.Quantum);
}