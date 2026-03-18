/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/serial.cpp

Description:
This file contains the
serial driver

Author:
UtsumiFuyuki
March 17th 2026
**/

#include <typedefs.hpp>
#include <hal/hal.hpp>
#include <hal/serial.hpp>

BOOL SerialEnabled = false;

VOID Hal::outb(UINT8 Data, UINT16 Port)
{
    __asm__ volatile ("outb %0, %1" :: "a"(Data), "Nd"(Port));
}

UINT8 Hal::inb(UINT16 Port)
{
    UINT8 Value;
    __asm__ volatile ("inb %1, %0" : "=a"(Value) : "Nd"(Port));
    return Value;
}

VOID Hal::SerialSetDlab(UINT16 Port, BOOL Setting)
{
    UINT8 LineControlRegister = inb(Port + SERIAL_REGISTER_LCR);
    
    if (Setting)
    {
        LineControlRegister |= LCR_DLAB;
    }
    else
    {
        LineControlRegister = LineControlRegister & ~LCR_DLAB;
    }

    outb(LineControlRegister, Port + SERIAL_REGISTER_LCR);
}

VOID Hal::SerialSetBaud(UINT16 Port, UINT16 BaudRate)
{
    outb((BaudRate & 0xFF), Port + SERIAL_REGISTER_DLAB_LO);
    outb((BaudRate >> 8) & 0xFF, Port + SERIAL_REGISTER_DLAB_HI);
}

VOID Hal::SerialSetLineCr(UINT16 Port, UINT8 LineControlRegister)
{
    outb(LineControlRegister, Port + SERIAL_REGISTER_LCR);
}

VOID Hal::SerialSetFifo(UINT16 Port, UINT8 Fifo)
{
    outb(Fifo, Port + SERIAL_REGISTER_FIFOCR);
}

VOID Hal::SerialSetMcr(UINT16 Port, UINT8 ModemControlRegister)
{
    outb(ModemControlRegister, Port + SERIAL_REGISTER_MCR);
}

VOID Hal::InitializeSerial(UINT16 Port)
{
    outb(0, Port + SERIAL_REGISTER_IER);
    Hal::SerialSetDlab(Port, true);
    Hal::SerialSetBaud(Port, BAUD_115200);
    Hal::SerialSetLineCr(Port, LCR_DATA_8 | LCR_PARITY_NONE | LCR_STOP_0);
    Hal::SerialSetFifo(Port, FIFO_ENABLE | FIFO_CLEAR_RECIEVE | FIFO_CLEAR_TRANSMIT | FIFO_ITL_14B);
    Hal::SerialSetMcr(Port, MCR_RTS | MCR_DTR | MCR_OUT2);
    Hal::SerialSetMcr(Port, MCR_RTS | MCR_OUT1 | MCR_OUT2 | MCR_LOOP);

    for (INT TryCount = 0; TryCount < 10; TryCount++)
    {
        outb(0xAE, Port + SERIAL_REGISTER_TB);

        if (inb(Port + SERIAL_REGISTER_RB) == 0xAE)
        {
            SerialEnabled = true;
            break;
        }
    }

    Hal::SerialSetMcr(Port, MCR_DTR | MCR_RTS | MCR_OUT1 | MCR_OUT2);
}

INT Hal::SerialRecieved(UINT16 Port)
{
    return inb(Port + SERIAL_REGISTER_LSR) & 0x1;
}

CHAR Hal::ReadSerial(UINT16 Port)
{
    if (SerialEnabled)
    {
        while (Hal::SerialRecieved(Port) == 0);

        return inb(Port);
    }
    return '\0';
}

INT Hal::TransmitEmpty(UINT16 Port)
{
    return inb(Port + SERIAL_REGISTER_LSR) & 0x20;
}

VOID Hal::WriteSerial(UINT16 Port, CHAR Character)
{
    if (SerialEnabled)
    {
        while (Hal::TransmitEmpty(Port) == 0);

        outb(Character, Port);
    }
}

VOID Hal::WriteStringToSerial(UINT16 Port, LPCSTR String)
{
    if (SerialEnabled)
    {
        for (int i = 0; String[i] != '\0'; i++)
        {
            Hal::WriteSerial(Port, String[i]);
        }
    }
}