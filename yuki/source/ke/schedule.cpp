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

extern "C" void contextSwitch(THREAD *oldThread, THREAD *currentThread);

extern "C" CIRCULAR_LIST<THREAD *> queue{};
extern "C" CPU_LOCAL cpuLocal;

LL_NODE<THREAD *> *currentThread = queue.getHead();

extern "C" void keSchedule() {
    if (queue.empty()) {
        ke::log(__FILE__, "No threads in queue!\r\n");
        return;
    }
    else {
        if (currentThread == nullptr) {
            currentThread = queue.getHead();
        }

        LL_NODE<THREAD *> *oldThread = currentThread;
        currentThread = currentThread->next;

        if (currentThread == oldThread) {
            ke::log(__FILE__, "currentThread is same as oldThread, returning...\r\n");
            return;
        }
        else {
            hal::setRsp0(currentThread->data->kstack);
            cpuLocal.currentThread = currentThread->data;
            contextSwitch(oldThread->data, currentThread->data);
        }
        return;
    }
}