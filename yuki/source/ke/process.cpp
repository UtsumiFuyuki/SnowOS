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
#include <ke/process.hpp>
#include <ke/string.hpp>
#include <mm/slab.hpp>
#include <mm/mm.hpp>
#include <utils/list.hpp>

uint64_t tids{};

extern "C" CIRCULAR_LIST<THREAD *> queue;

// TODO: Create arch agnositc abstraction for this
extern "C" void setupStack(THREAD *thread);

void ke::addThreadToQueue(THREAD *thread) {
    auto *listNode = reinterpret_cast<LL_NODE<THREAD *>*>(mm::allocatePool(sizeof(LL_NODE<THREAD *>)));
    listNode->data = thread;
    queue.push(listNode);
}

THREAD *ke::createThread(void (*entry)(void *), THREAD_TYPE threadType, PROCESS *parent) {
    auto *thread = reinterpret_cast<THREAD *>(mm::allocatePool(sizeof(THREAD)));
    memset(thread, 0, sizeof(THREAD));

    thread->rip = reinterpret_cast<uint64_t>(entry);
    
    switch (threadType) {
        case (THREAD_TYPE::KERNEL):
            thread->rsp = reinterpret_cast<uint64_t>(mm::allocateKernelPages(8)) + 0x8000;
            break;
        case (THREAD_TYPE::USER):
            thread->rsp = reinterpret_cast<uint64_t>(mm::allocateUserPages(8)) + 0x8000;
            break;
    }
    thread->kstack = reinterpret_cast<uint64_t>(mm::allocateKernelPages(4)) + 0x4000;

    thread->parentProc = parent;
    thread->tid = tids;

    tids++;

    setupStack(thread);
    addThreadToQueue(thread);

    return thread;
}