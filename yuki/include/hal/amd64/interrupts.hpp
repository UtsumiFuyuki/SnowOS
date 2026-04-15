/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025
 
File: hal/amd64/stack_frame.hpp

Description:
IDT Stack Frame

Author:
UtsumiFuyuki
April 15th 2026
**/

#pragma once

#include <cstddef>
#include <cstdint>

typedef struct _INTERRUPT_REGISTERS {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
} __attribute__((packed)) INTERRUPT_REGISTERS;

typedef struct _CPU_STACK_FRAME {
    uint64_t errorCode;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} CPU_STACK_FRAME;

typedef struct _INTERRUPT_FRAME {
    INTERRUPT_REGISTERS registers;
    uint64_t vector;
    CPU_STACK_FRAME cpuRegisters;
} INTERRUPT_FRAME;

static_assert(offsetof(INTERRUPT_FRAME, registers) == 0);
static_assert(offsetof(INTERRUPT_FRAME, vector) == 120);
static_assert(offsetof(INTERRUPT_FRAME, cpuRegisters) == 128);

extern "C" [[noreturn]] void keInterruptHandler(INTERRUPT_REGISTERS* savedRegisters,
                                                CPU_STACK_FRAME *cpuSavedRegisters,
                                                uint64_t interruptVector);