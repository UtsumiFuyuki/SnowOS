/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: ke/schedule.cpp

Description:
The scheduler for Yuki

Author:
UtsumiFuyuki
April 28th 2026
**/

#include <ke/log.hpp>
#include <ke/process.hpp>
#include <hal/hal.hpp>
#include <hal/amd64/cpu_local.hpp>
#include <utils/list.hpp>

extern "C" CIRCULAR_LIST<THREAD *> queue{};
LL_NODE<THREAD *> *currentThread{};

extern "C" bool preemption{false};

extern "C" void enablePreemption() {
    preemption = true;
}

extern "C" void disablePreemption() {
    preemption = false;
}

extern "C" void contextSwitch(THREAD *oldThread, THREAD *newThread);

extern "C" void keSchedule() {
    if (queue.empty())
        return;
    else if (currentThread == nullptr)
        currentThread = queue.getHead();

    LL_NODE<THREAD *> *oldThread = currentThread;
    currentThread = currentThread->next;

    if (currentThread == oldThread)
        return;

    contextSwitch(oldThread->data, currentThread->data);
}