#include <hal/amd64/cpuid.hpp>

CPUID hal::x64::getCpuid(uint64_t rax) {
    CPUID cpuid{};

    __asm__ volatile (
                    "mov %0, %%rax;"
                    "cpuid;"
                    "mov %%rbx, %1;"
                    "mov %%rcx, %2;"
                    "mov %%rdx, %3;" : "=b"(cpuid.rbx), "=c"(cpuid.rcx), "=d"(cpuid.rdx) : "a"(rax));
    return cpuid;
}