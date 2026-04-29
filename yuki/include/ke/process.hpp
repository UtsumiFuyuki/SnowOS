/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: ke/process.hpp

Description:
Process and Thread structures

Author:
UtsumiFuyuki
April 26th 2026
**/

#pragma once

#include <cstdint>
#include <cstddef>

typedef struct _THREAD {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rbp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;

    uint64_t rsp;
    uint64_t rip;

    uint64_t cr3;
    uint64_t tid;

    uint64_t kstack;
} THREAD;

typedef struct _PROCESS {
    uint64_t pid;
    THREAD mainThread;
} PROCESS;

static_assert(offsetof(PROCESS, mainThread) == 8, "Offset of mainThread in PROCESS is incorrect!");

namespace ke {
    PROCESS *createProcess(void (*entry)(void *));
    void addThreadToQueue(THREAD *thread);
}