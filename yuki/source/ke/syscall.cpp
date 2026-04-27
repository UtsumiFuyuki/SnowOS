/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: ke/syscall.cpp

Description:
System call handler

Author:
UtsumiFuyuki
April 27th 2026
**/

#include <cstdint>
#include <ke/log.hpp>

extern "C" void keSyscallHandler() {
    uint64_t syscallNumber{};
    __asm__ volatile ("" : "=a"(syscallNumber));
    ke::print("System Call Invoked! Syscall %llu\r\n", syscallNumber);
}