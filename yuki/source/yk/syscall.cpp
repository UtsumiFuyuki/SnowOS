/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: yk/syscall.cpp

Description:
System call handler and initialization

Author:
UtsumiFuyuki
April 27th 2026
**/

#include <cstdint>
#include <hal/amd64/syscall.hpp>
#include <ke/log.hpp>
#include <yk/syscall.hpp>

void (*syscallTable[128])(SYSCALL_REGISTERS *regs){};

void ke::initializeSyscalls() {
    hal::x64::initSyscall();

    syscallTable[0] = ykTerminate;
    syscallTable[1] = ykWriteToConsole;
}

extern "C" void keSyscallHandler(SYSCALL_REGISTERS *regs) {
    uint64_t syscallNumber{};
    __asm__ volatile ("" : "=a"(syscallNumber));

    if (syscallNumber > 128) {
        ke::log(__FILE__, ANSI_RED "Invalid syscall number!\r\n" ANSI_RESET);
        return;
    }

    else if (syscallTable[syscallNumber] == nullptr) {
        ke::log(__FILE__, "Syscall %llu is not implemented!\r\n", syscallNumber);
        return;
    }

    syscallTable[syscallNumber](regs);
}

void ykWriteToConsole(SYSCALL_REGISTERS *regs) {
    // TODO: sanatize string
    const char *string = reinterpret_cast<const char *>(regs->rdx);
    ke::print("%s", string);
}

void ykTerminate(SYSCALL_REGISTERS *regs) {
    ke::log(__FILE__, "Unimplemented!\r\n");
}