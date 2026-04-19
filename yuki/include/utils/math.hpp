/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: utils/math.hpp

Description:
Math functions

Author:
UtsumiFuyuki
April 19th 2026
**/

#pragma once

#include <cstdint>
#include <bit>
#include <ke/string.hpp>
#include <ke/log.hpp>

static inline uint64_t pow(uint64_t base, uint64_t exponent) {
    uint64_t ret{1};

    for (size_t i = 1; i <= exponent; i++) {
        ret *= base;
    }
    return ret;
}

static inline uint64_t log2(const uint64_t &x) {
    return 64 - std::countl_zero(x) - 1;
}

static inline uint64_t atoi(const char *string) {
    size_t length = strlen(string) - 1;
    size_t n = pow(10, length - 1);
    uint64_t ret{};

    int k = 0;
    while (length != 0 && n != 0) {
        ret += (string[k] - '0') * n;
        n /= 10;
        k++;
    }
    return ret;
}