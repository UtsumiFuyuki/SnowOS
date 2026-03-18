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

#include <typedefs.hpp>
#include <ke/string.hpp>

extern "C" {

    LPVOID memcpy(LPVOID __restrict dest, LPCVOID __restrict src, UINT64 n) {
        UINT8 *__restrict pdest = static_cast<UINT8 *__restrict>(dest);
        const UINT8 *__restrict psrc = static_cast<const UINT8 *__restrict>(src);
    
        for (UINT64 i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    
        return dest;
    }
    
    LPVOID memset(LPVOID s, INT c, UINT64 n) {
        UINT8 *p = static_cast<UINT8 *>(s);
    
        for (UINT64 i = 0; i < n; i++) {
            p[i] = static_cast<UINT8>(c);
        }
    
        return s;
    }
    
    LPVOID memmove(LPVOID dest, LPCVOID src, UINT64 n) {
        UINT8 *pdest = static_cast<UINT8 *>(dest);
        const UINT8 *psrc = static_cast<const UINT8 *>(src);
    
        if (src > dest) {
            for (UINT64 i = 0; i < n; i++) {
                pdest[i] = psrc[i];
            }
        } else if (src < dest) {
            for (UINT64 i = n; i > 0; i--) {
                pdest[i-1] = psrc[i-1];
            }
        }
    
        return dest;
    }
    
    INT memcmp(LPCVOID s1, LPCVOID s2, UINT64 n) {
        const UINT8 *p1 = static_cast<const UINT8 *>(s1);
        const UINT8 *p2 = static_cast<const UINT8 *>(s2);
    
        for (UINT64 i = 0; i < n; i++) {
            if (p1[i] != p2[i]) {
                return p1[i] < p2[i] ? -1 : 1;
            }
        }
    
        return 0;
    }
}

UINT64 strlen(LPCSTR String) {
        UINT64 Length = 0;
        for (INT i = 0; String[i] != '\0'; i++) {
            Length++;
        } Length++;
        return Length;
    }