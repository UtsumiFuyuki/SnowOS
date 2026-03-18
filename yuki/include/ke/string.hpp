/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025
 
File: ke/string.hpp

Description:
This file contains the function
prototypes for some of the functions from
the string library

Author:
UtsumiFuyuki
October 30th 2025
**/

#pragma once

#include <typedefs.hpp>

extern "C" LPVOID memcpy(LPVOID __restrict dest, LPCVOID __restrict src, UINT64 n);
extern "C" LPVOID memset(LPVOID s, INT c, UINT64 n);
extern "C" LPVOID memmove(LPVOID dest, LPCVOID src, UINT64 n);
extern "C" INT memcmp(LPCVOID s1, LPCVOID s2, UINT64 n);
UINT64 strlen(LPCSTR String);