#include <hal/amd64/cpuid.hpp>

CPUID Hal::X64::GetCpuid(UINT64 Rax)
{
    CPUID Cpuid{};

    __asm__ volatile (
                    "mov %0, %%rax;"
                    "cpuid;"
                    "mov %%rbx, %1;"
                    "mov %%rcx, %2;"
                    "mov %%rdx, %3;" : "=b"(Cpuid.Rbx), "=c"(Cpuid.Rcx), "=d"(Cpuid.Rdx) : "a"(Rax));
    return Cpuid;
}