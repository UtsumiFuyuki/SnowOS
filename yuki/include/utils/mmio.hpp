/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: utils/mmio.hpp

Description:
MMIO helper functions

Author:
UtsumiFuyuki
April 25th 2026
**/

#pragma once

#include <cstdint>

static inline void mmioWrite64(uint64_t *address, uint64_t value) {
    __asm__ volatile ("movq %0, %1" :: "a"(value), "o"(*address) : "memory");
}

static inline uint64_t mmioRead64(uint64_t *address) {
    uint64_t ret{};
    __asm__ volatile ("movq %1, %0" : "=a"(ret) : "o"(*address) : "memory");
    return ret;
}

static inline void mmioWrite32(uint64_t *address, uint32_t value) {
    __asm__ volatile ("movl %0, %1" :: "a"(value), "o"(*address) : "memory");
}

static inline uint64_t mmioRead32(uint64_t *address) {
    uint32_t ret{};
    __asm__ volatile ("movl %1, %0" : "=a"(ret) : "o"(*address) : "memory");
    return ret;
}

static inline void mmioWrite16(uint64_t address, uint64_t reg, uint16_t value) {
    *reinterpret_cast<uint16_t *>(address + reg) = value;
}

static inline uint64_t mmioRead16(uint64_t address, uint64_t reg) {
    return *reinterpret_cast<uint16_t *>(address + reg);
}

static inline void mmioWrite8(uint64_t address, uint64_t reg, uint8_t value) {
    *reinterpret_cast<uint8_t *>(address + reg) = value;
}

static inline uint64_t mmioRead8(uint64_t address, uint64_t reg) {
    return *reinterpret_cast<uint8_t *>(address + reg);
}