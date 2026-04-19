/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025
 
File: ke/spinlock.hpp

Description:
The Spinlock implementation for Yuki

Author:
UtsumiFuyuki
March 24th 2026
**/

#pragma once

#include <cstdint>
#include <hal/hal.hpp>

typedef struct _SPINLOCK
{
    uint64_t flag{};
} SPINLOCK, *PSPINLOCK;

namespace ke {
    static inline void spinlockInitialize(PSPINLOCK lock) {
        __atomic_store_n(&lock->flag, 0, __ATOMIC_RELAXED);
    }

    [[nodiscard]]
    static inline bool spinlockAcquire(PSPINLOCK lock) {
        // Get the state of the IF before disabling interrupts
        bool intsEnabled = halInterruptsEnabled();

        __asm__ volatile ("cli" ::: "memory");

        while (true) {
            if (!__atomic_exchange_n(&lock->flag, 1, __ATOMIC_ACQUIRE))
                break;

            while (__atomic_load_n(&lock->flag, __ATOMIC_RELAXED))
                __asm__ volatile ("pause" ::: "memory");
        }

        return intsEnabled;
    }

    static inline void spinlockRelease(PSPINLOCK lock, bool intsEnabled) {
        __atomic_store_n(&lock->flag, 0, __ATOMIC_RELEASE);
        if (intsEnabled)
            __asm__ volatile ("sti" ::: "memory");

        else
            __asm__ volatile ("cli" ::: "memory");
    }
}