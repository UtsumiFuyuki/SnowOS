/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: yk/syscall.cpp

Description:
Prototypes for system calls

Author:
UtsumiFuyuki
April 27th 2026
**/

#pragma once

#include <hal/amd64/interrupts.hpp>
typedef INTERRUPT_REGISTERS SYSCALL_REGISTERS;

namespace ke {
    void initializeSyscalls();
}

void ykWriteToConsole(SYSCALL_REGISTERS *regs);
void ykTerminate(SYSCALL_REGISTERS *regs);