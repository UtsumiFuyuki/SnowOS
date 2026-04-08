/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025
 
File: ke/print.cpp

Description:
This file contains the logging
functions for Yuki

Author:
UtsumiFuyuki
October 30th 2025
**/

#include <cstdarg>
#include <ke/string.hpp>
#include <hal/hal.hpp>
#include <hal/serial.hpp>
#include <ke/log.hpp>

#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

// Compile nanoprintf in this translation unit.
#define NANOPRINTF_IMPLEMENTATION
#include <ke/nanoprintf.hpp>

char buffer[512];
char loggerBuffer[512];

void ke::log(const char *file, const char *string, ...) {
    va_list arguments;
    va_start(arguments, string);

    hal::writeStringToSerial(COM1, file);
    hal::writeStringToSerial(COM1, ": ");

    npf_vsnprintf(loggerBuffer, sizeof(loggerBuffer), string, arguments);

    hal::writeStringToSerial(COM1, loggerBuffer);

    va_end(arguments);

    memset(loggerBuffer, 0, sizeof(loggerBuffer));
}

void ke::print(const char *string, ...) {
    va_list arguments;
    va_start(arguments, string);

    npf_vsnprintf(buffer, sizeof(buffer), string, arguments);
    hal::printString(buffer);

    va_end(arguments);
    memset(buffer, 0, sizeof(buffer));
}