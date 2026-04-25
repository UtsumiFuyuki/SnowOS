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

#include <cstdint>

extern "C" void *memcpy(void *__restrict dest, const void *__restrict src, uint64_t n);
extern "C" void *memset(void *s, int c, uint64_t n);
extern "C" void *memmove(void *dest, const void *src, uint64_t n);
extern "C" int memcmp(const void *s1, const void *s2, uint64_t n);
uint64_t strlen(const char *string);
int strncmp(const char* str1, const char *str2, size_t length);