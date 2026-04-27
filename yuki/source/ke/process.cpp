/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: ke/process.cpp

Description:
Process and Thread structures

Author:
UtsumiFuyuki
April 26th 2026
**/

#include <cstdint>
#include <hal/hal.hpp>
#include <hal/paging.hpp>
#include <ke/process.hpp>
#include <ke/string.hpp>
#include <mm/mm.hpp>
#include <mm/slab.hpp>

// Mmm, bump allocating pids
uint64_t pids{};
uint64_t tids{};

PROCESS *ke::createProcess(void (*entry)(void *)) {
    auto *proc = new PROCESS;

    proc->pid = pids;
    pids++;

    proc->mainThread.rip = reinterpret_cast<uint64_t>(entry);
    proc->mainThread.rsp = reinterpret_cast<uint64_t>(mm::allocateUserPages(8)) + 0x8000;
    proc->mainThread.kstack = reinterpret_cast<uint64_t>(mm::allocateKernelPages(4)) + 0x4000;
    proc->mainThread.cr3 = reinterpret_cast<uint64_t>(mm::allocatePage());
    proc->mainThread.tid = tids;
    tids++;

    hal::mapPages(0x1000, 0x20000, 0x6000, PAGE_USER | PAGE_WRITE, reinterpret_cast<uint64_t *>(proc->mainThread.cr3 + hal::retrieveHhdmOffset()));

    memcpy(reinterpret_cast<uint64_t *>(proc->mainThread.cr3 + hal::retrieveHhdmOffset()) + 256, hal::getKernelPagemap() + 256, 2048);

    return proc;
}