/**
Snow Operating System
Copyright (c) BlueSillyDragon 2025
 
File: ke/string.cpp

Description:
This file contains the function
definitions for some of the functions from
the string library

Author:
BlueSillyDragon
October 30th 2025
**/

#include <ke/string.hpp>

extern "C" {
    
    void *memcpy(void *__restrict dest, const void *__restrict src, uint64_t n) {
        uint8_t *__restrict pdest = static_cast<uint8_t *__restrict>(dest);
        const uint8_t *__restrict psrc = static_cast<const uint8_t *__restrict>(src);
    
        for (uint64_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    
        return dest;
    }
    
    // TODO: Use stosb
    void *memset(void *s, int c, uint64_t n) {
        uint8_t *p = static_cast<uint8_t *>(s);
    
        for (uint64_t i = 0; i < n; i++) {
            p[i] = static_cast<uint8_t>(c);
        }
    
        return s;
    }
    
    void *memmove(void *dest, const void *src, uint64_t n) {
        uint8_t *pdest = static_cast<uint8_t *>(dest);
        const uint8_t *psrc = static_cast<const uint8_t *>(src);
    
        if (src > dest) {
            for (uint64_t i = 0; i < n; i++) {
                pdest[i] = psrc[i];
            }
        } else if (src < dest) {
            for (uint64_t i = n; i > 0; i--) {
                pdest[i-1] = psrc[i-1];
            }
        }
    
        return dest;
    }
    
    int memcmp(const void *s1, const void *s2, uint64_t n) {
        const uint8_t *p1 = static_cast<const uint8_t *>(s1);
        const uint8_t *p2 = static_cast<const uint8_t *>(s2);
    
        for (uint64_t i = 0; i < n; i++) {
            if (p1[i] != p2[i]) {
                return p1[i] < p2[i] ? -1 : 1;
            }
        }
    
        return 0;
    }
}

uint64_t strlen(const char *string) {
        size_t length{};
        for (size_t i = 0; string[i] != '\0'; i++) {
            length++;
        } length++;
        return length;
    }