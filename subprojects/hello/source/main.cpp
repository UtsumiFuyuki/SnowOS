#include <syscalls.hpp>

__declspec(allocate(".CRT$XCA")) const void *crt_xct = nullptr;
__declspec(allocate(".CRT$XCZ")) const void *crt_xcz = nullptr;

extern "C" void runConstructors() {
	using initializerPtr = void (*)();
	long long begin = reinterpret_cast<long long>(&crt_xct);
	long long end = reinterpret_cast<long long>(&crt_xcz);
	for (long long it = begin + sizeof(void *); it < end; it += sizeof(void *)) {
		auto *p = reinterpret_cast<initializerPtr *>(it);
		(*p)();
	}
}

extern "C" void _start() {
    runConstructors();
	YkWriteToConsole("aomi");
    for (;;) {
        __asm__ volatile ("mov $0xcafebabe, %rax");
    }
}