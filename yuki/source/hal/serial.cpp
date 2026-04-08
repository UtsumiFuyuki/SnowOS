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

#include <hal/hal.hpp>
#include <hal/serial.hpp>

bool serialEnabled = false;

void hal::outb(uint8_t data, uint16_t port) {
    __asm__ volatile ("outb %0, %1" :: "a"(data), "Nd"(port));
}

uint8_t hal::inb(uint16_t port) {
    uint8_t Value;
    __asm__ volatile ("inb %1, %0" : "=a"(Value) : "Nd"(port));
    return Value;
}

void hal::serialSetDlab(uint16_t port, bool setting) {
    uint8_t lineControlRegister = inb(port + SERIAL_REGISTER_LCR);
    
    if (setting) {
        lineControlRegister |= LCR_DLAB;
    }
    else {
        lineControlRegister = lineControlRegister & ~LCR_DLAB;
    }

    outb(lineControlRegister, port + SERIAL_REGISTER_LCR);
}

void hal::serialSetBaud(uint16_t port, uint16_t BaudRate) {
    outb((BaudRate & 0xFF), port + SERIAL_REGISTER_DLAB_LO);
    outb((BaudRate >> 8) & 0xFF, port + SERIAL_REGISTER_DLAB_HI);
}

void hal::serialSetLineCr(uint16_t port, uint8_t lineControlRegister) {
    outb(lineControlRegister, port + SERIAL_REGISTER_LCR);
}

void hal::serialSetFifo(uint16_t port, uint8_t fifo) {
    outb(fifo, port + SERIAL_REGISTER_FIFOCR);
}

void hal::serialSetMcr(uint16_t port, uint8_t ModemControlRegister) {
    outb(ModemControlRegister, port + SERIAL_REGISTER_MCR);
}

void hal::initializeSerial(uint16_t port) {
    outb(0, port + SERIAL_REGISTER_IER);
    hal::serialSetDlab(port, true);
    hal::serialSetBaud(port, BAUD_115200);
    hal::serialSetLineCr(port, LCR_DATA_8 | LCR_PARITY_NONE | LCR_STOP_0);
    hal::serialSetFifo(port, FIFO_ENABLE | FIFO_CLEAR_RECIEVE | FIFO_CLEAR_TRANSMIT | FIFO_ITL_14B);
    hal::serialSetMcr(port, MCR_RTS | MCR_DTR | MCR_OUT2);
    hal::serialSetMcr(port, MCR_RTS | MCR_OUT1 | MCR_OUT2 | MCR_LOOP);

    for (size_t tryCount = 0; tryCount < 10; tryCount++)
    {
        outb(0xAE, port + SERIAL_REGISTER_TB);

        if (inb(port + SERIAL_REGISTER_RB) == 0xAE)
        {
            serialEnabled = true;
            break;
        }
    }

    hal::serialSetMcr(port, MCR_DTR | MCR_RTS | MCR_OUT1 | MCR_OUT2);
}

int hal::serialRecieved(uint16_t port) {
    return inb(port + SERIAL_REGISTER_LSR) & 0x1;
}

char hal::readSerial(uint16_t port) {
    if (serialEnabled)
    {
        while (hal::serialRecieved(port) == 0);

        return inb(port);
    }
    return '\0';
}

int hal::transmitEmpty(uint16_t port) {
    return inb(port + SERIAL_REGISTER_LSR) & 0x20;
}

void hal::writeSerial(uint16_t port, char character) {
    if (serialEnabled) {
        while (hal::transmitEmpty(port) == 0);
        outb(character, port);
    }
}

void hal::writeStringToSerial(uint16_t port, const char *string) {
    if (serialEnabled) {
        for (int i = 0; string[i] != '\0'; i++) {
            hal::writeSerial(port, string[i]);
        }
    }
}