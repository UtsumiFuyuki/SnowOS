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
#include <ke/print.hpp>

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

char Buffer[512];
int Index = 0;

void Ke::Print(LOG_TYPE LogType, const char *String, ...)
{
    va_list Arguments;
    va_start(Arguments, String);

    switch (LogType)
    {
        case (LOG_TYPE::KeLog):
            Hal::PrintString("[" ANSI_BRIGHT_CYAN "Ke" ANSI_RESET "] ");
            break;
        case (LOG_TYPE::HalLog):
            Hal::PrintString("[" ANSI_BLUE "Hal" ANSI_RESET "] ");
            break;
        case (LOG_TYPE::None):
            break;
    }

    npf_vsnprintf(Buffer, sizeof(Buffer), String, Arguments);

    Hal::PrintString(Buffer);

    va_end(Arguments);

    memset(Buffer, 0, sizeof(Buffer));
    Index = 0;
}