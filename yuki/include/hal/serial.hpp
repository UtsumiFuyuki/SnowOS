/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: hal/serial.hpp

Description:
This file contains macros
and definitions for the serial driver

Author:
UtsumiFuyuki
March 17th 2026
**/

#pragma once

#include <typedefs.hpp>

// Serial Port Registers
#define SERIAL_REGISTER_RB 0 // Recieve Buffer | Read
#define SERIAL_REGISTER_TB 0 // Transmit Buffer | Write
#define SERIAL_REGISTER_IER 1 // Interrupt Enable Register | Read/Write
#define SERIAL_REGISTER_DLAB_LO 0
#define SERIAL_REGISTER_DLAB_HI 1
#define SERIAL_REGISTER_II 2 // Interrupt Identification | Read
#define SERIAL_REGISTER_FIFOCR 2 // FIFO Control Registers | Write
#define SERIAL_REGISTER_LCR 3 // Line Control Registers | Read/Write
#define SERIAL_REGISTER_MCR 4 // Modem Control Register | Read/Write
#define SERIAL_REGISTER_LSR 5 // Line Status Register | Read
#define SERIAL_REGISTER_MSR 6 // Modem Status Register | Read
#define SERIAL_REGISTER_SR 7 // Scratch Register | Read

// Baud Rates
#define BAUD_115200 1
#define BAUD_57600 2
#define BAUD_38400 3
#define BAUD_19200 6
#define BAUD_9600 12

// Line Control Register Bits
#define LCR_DLAB 0x80
#define LCR_BEB 0x40
#define LCR_STOP_0 0x0
#define LCR_STOP_1 0x4
#define LCR_DATA_5 0x0
#define LCR_DATA_6 0x1
#define LCR_DATA_7 0x2
#define LCR_DATA_8 0x3
#define LCR_PARITY_NONE 0x0
#define LCR_PARTY_ODD 0x8
#define LCR_PARITY_EVEN 0x18
#define LCR_PARITY_MARK 0x28
#define LCR_PARITY_SPACE 0x38

// FIFO Control Register Bits
#define FIFO_ENABLE 0x1
#define FIFO_DISABLE 0x0
#define FIFO_CLEAR_RECIEVE 0x2
#define FIFO_CLEAR_TRANSMIT 0x4
#define FIFO_DMA_MODE 0x8
#define FIFO_ITL_1B 0x0
#define FIFO_ITL_4B 0x40
#define FIFO_ITL_8B 0x80
#define FIFO_ITL_14B 0xC0

// Modem Control Register Bits
#define MCR_DTR 0x1
#define MCR_RTS 0x2
#define MCR_OUT1 0x4
#define MCR_OUT2 0x8
#define MCR_LOOP 0x10

// Ports 
#define COM1 0x3F8
#define COM2 0x2F8

namespace Hal
{
    inline VOID outb(UINT8 Data, UINT16 Port);
    inline UINT8 inb(UINT16 Port);

    VOID SerialSetDlab(UINT16 Port, BOOL Setting);
    VOID SerialSetBaud(UINT16 Port, UINT16 BaudRate);
    VOID SerialSetLineCr(UINT16 Port, UINT8 LineControlRegister);
    VOID SerialSetFifo(UINT16 Port, UINT8 Fifo);
    VOID SerialSetMcr(UINT16 Port, UINT8 ModemControlRegister);

    INT SerialRecieved(UINT16 Port);
    CHAR ReadSerial(UINT16 Port);
    INT TransmitEmpty(UINT16 Port);
    VOID WriteSerial(UINT16 Port, CHAR Character);

    VOID WriteStringToSerial(UINT16 Port, LPCSTR String);

    VOID InitializeSerial(UINT16 Port);
}