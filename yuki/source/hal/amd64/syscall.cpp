/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025
 
File: hal/amd64/syscall.cpp

Description:
Function for setting up SYSCALL/SYSRET

Author:
UtsumiFuyuki
October 28th 2025
**/

#include <hal/amd64/syscall.hpp>
#include <hal/amd64/msr.hpp>
#include <hal/amd64/cpu_local.hpp>
#include <ke/log.hpp>

extern "C" void syscallHandler();

void hal::x64::initSyscall() {
    ke::log(__FILE__, "Setting up syscalls...\r\n");

    uint64_t efer = rdmsr(IA32_EFER);
    efer |= (1 << 0);
    wrmsr(IA32_EFER, efer);

    uint64_t lstar = reinterpret_cast<uint64_t>(syscallHandler);
    wrmsr(IA32_LSTAR, lstar);
    uint64_t star = (static_cast<uint64_t>(0x18 | 3) << 48) | (static_cast<uint64_t>(0x08) << 32);
    wrmsr(IA32_STAR, star);

    ke::print("Initialized syscalls!\r\n");
}

void hal::x64::setCpuLocal(CPU_LOCAL *cpuLocal) {
    uint64_t kernelGsBase = rdmsr(IA32_KERNEL_GS_BASE);
    kernelGsBase = reinterpret_cast<uint64_t>(cpuLocal);
    wrmsr(IA32_KERNEL_GS_BASE, kernelGsBase);

    ke::log(__FILE__, "Kernel GS Base now set to: 0x%llX\r\n", rdmsr(IA32_KERNEL_GS_BASE));
}