/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/amd64/syscall.hpp

Description:
Function prototypes for setting up SYSCALL/SYSRET

Author:
UtsumiFuyuki
April 27th 2026
**/

#pragma once

namespace hal {
    namespace x64 {
        void initSyscall();
    }
}