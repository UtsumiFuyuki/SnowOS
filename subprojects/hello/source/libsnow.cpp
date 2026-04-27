#include <syscalls.hpp>

void YkWriteToConsole(const char *string) {
    __asm__ volatile ("syscall" :: "a"(1));
}