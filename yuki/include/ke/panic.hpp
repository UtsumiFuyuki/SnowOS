/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: ke/panic.hpp

Description:
Panic function prototype

Author:
UtsumiFuyuki
July 11th 2026
**/

#pragma once

#include <hal/amd64/interrupts.hpp>

extern "C" [[noreturn]] void kePanic(const char *reason, INTERRUPT_FRAME *intFrame = nullptr);