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

#include "hal/serial.hpp"
#include <typedefs.hpp>
#include <cstdarg>
#include <ke/string.hpp>
#include <hal/hal.hpp>
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

CHAR Buffer[512];
CHAR LoggerBuffer[512];

VOID Ke::Log(LPCSTR File, LPCSTR String, ...)
{
    va_list Arguments;
    va_start(Arguments, String);

    Hal::WriteStringToSerial(COM1, File);
    Hal::WriteStringToSerial(COM1, ": ");

    npf_vsnprintf(LoggerBuffer, sizeof(LoggerBuffer), String, Arguments);

    Hal::WriteStringToSerial(COM1, LoggerBuffer);

    va_end(Arguments);

    memset(LoggerBuffer, 0, sizeof(LoggerBuffer));
}

VOID Ke::Print(LPCSTR String, ...)
{
    va_list Arguments;
    va_start(Arguments, String);

    npf_vsnprintf(Buffer, sizeof(Buffer), String, Arguments);

    Hal::PrintString(Buffer);

    va_end(Arguments);

    memset(Buffer, 0, sizeof(Buffer));
}