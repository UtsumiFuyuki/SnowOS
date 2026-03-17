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

#include <cstdint>
#include <hal/hal.hpp>
#include <hal/serial.hpp>

bool SerialEnabled = false;

void Hal::outb(uint8_t Data, uint16_t Port)
{
    __asm__ volatile ("outb %0, %1" :: "a"(Data), "Nd"(Port));
}

uint8_t Hal::inb(uint16_t Port)
{
    uint8_t Value;
    __asm__ volatile ("inb %1, %0" : "=a"(Value) : "Nd"(Port));
    return Value;
}

void Hal::SerialSetDlab(uint16_t Port, bool Setting)
{
    uint8_t LineControlRegister = inb(Port + SERIAL_REGISTER_LCR);
    
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

void Hal::SerialSetBaud(uint16_t Port, uint16_t BaudRate)
{
    outb((BaudRate & 0xFF), Port + SERIAL_REGISTER_DLAB_LO);
    outb((BaudRate >> 8) & 0xFF, Port + SERIAL_REGISTER_DLAB_HI);
}

void Hal::SerialSetLineCr(uint16_t Port, uint8_t LineControlRegister)
{
    outb(LineControlRegister, Port + SERIAL_REGISTER_LCR);
}

void Hal::SerialSetFifo(uint16_t Port, uint8_t Fifo)
{
    outb(Fifo, Port + SERIAL_REGISTER_FIFOCR);
}

void Hal::SerialSetMcr(uint16_t Port, uint8_t ModemControlRegister)
{
    outb(ModemControlRegister, Port + SERIAL_REGISTER_MCR);
}

void Hal::InitializeSerial(uint16_t Port)
{
    outb(0, Port + SERIAL_REGISTER_IER);
    Hal::SerialSetDlab(Port, true);
    Hal::SerialSetBaud(Port, BAUD_115200);
    Hal::SerialSetLineCr(Port, LCR_DATA_8 | LCR_PARITY_NONE | LCR_STOP_0);
    Hal::SerialSetFifo(Port, FIFO_ENABLE | FIFO_CLEAR_RECIEVE | FIFO_CLEAR_TRANSMIT | FIFO_ITL_14B);
    Hal::SerialSetMcr(Port, MCR_RTS | MCR_DTR | MCR_OUT2);
    Hal::SerialSetMcr(Port, MCR_RTS | MCR_OUT1 | MCR_OUT2 | MCR_LOOP);

    for (int TryCount = 0; TryCount < 10; TryCount++)
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

int Hal::SerialRecieved(uint16_t Port)
{
    return inb(Port + SERIAL_REGISTER_LSR) & 0x1;
}

char Hal::ReadSerial(uint16_t Port)
{
    if (SerialEnabled)
    {
        while (Hal::SerialRecieved(Port) == 0);

        return inb(Port);
    }
    return '\0';
}

int Hal::TransmitEmpty(uint16_t Port)
{
    return inb(Port + SERIAL_REGISTER_LSR) & 0x20;
}

void Hal::WriteSerial(uint16_t Port, char Character)
{
    if (SerialEnabled)
    {
        while (Hal::TransmitEmpty(Port) == 0);

        outb(Character, Port);
    }
}

void Hal::WriteStringToSerial(uint16_t Port, const char *String)
{
    if (SerialEnabled)
    {
        for (int i = 0; String[i] != '\0'; i++)
        {
            Hal::WriteSerial(Port, String[i]);
        }
    }
}