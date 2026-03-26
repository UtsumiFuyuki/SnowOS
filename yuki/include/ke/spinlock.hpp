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

#include <hal/hal.hpp>

typedef struct _SPINLOCK
{
    UINT64 Flag{};
} SPINLOCK, *PSPINLOCK;

namespace Ke
{
    static inline VOID SpinlockInitialize(PSPINLOCK Lock)
    {
        __atomic_store_n(&Lock->Flag, 0, __ATOMIC_RELAXED);
    }

    [[nodiscard]]
    static inline BOOL SpinlockAcquire(PSPINLOCK Lock)
    {
        // Get the state of the IF before disabling interrupts
        BOOL IntsEnabled = HalInterruptsEnabled();

        __asm__ volatile ("cli" ::: "memory");

        while (true)
        {
            if (!__atomic_exchange_n(&Lock->Flag, 1, __ATOMIC_ACQUIRE))
                break;

            while (__atomic_load_n(&Lock->Flag, __ATOMIC_RELAXED))
                __asm__ volatile ("pause" ::: "memory");
        }

        return IntsEnabled;
    }

    static inline VOID SpinlockRelease(PSPINLOCK Lock, BOOL IntsEnabled)
    {
        __atomic_store_n(&Lock->Flag, 0, __ATOMIC_RELEASE);
        if (IntsEnabled)
            __asm__ volatile ("sti" ::: "memory");

        else
            __asm__ volatile ("cli" ::: "memory");
    }
}