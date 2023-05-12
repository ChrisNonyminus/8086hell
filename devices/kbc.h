#ifndef __DEVICES_KeyboardController_H__
#define __DEVICES_KeyboardController_H__

#include "../x86.h"
#include <cstdint>

struct X86_GenericKeyboardController : public X86_IODeviceBase {
    enum class Status {
        OUTPUT_BUFFER_STATUS = (1 << 0),
        INPUT_BUFFER_STATUS = (1 << 1),
        SYSTEM_FLAG = (1 << 2),
        COMMAND_DATA = (1 << 3),
        TIMEOUT_ERROR = (1 << 6),
        PARITY_ERROR = (1 << 7)
        
    };
    int status{0};

    enum class Command {
        READ_RAM_OFF0 = 0x20,
        READ_RAM_OFF1 = 0x21,
        // ... (read 2-1E)
        READ_RAM_OFF1F = 0x3F,
        WRITE_RAM_OFF0 = 0x60,
        WRITE_RAM_OFF1 = 0x61,
        // ... (write 2-1E)
        WRITE_RAM_OFF1F = 0x7F,
        
        DISABLE_SECOND_PS2PORT = 0xA7,
        ENABLE_SECOND_PS2PORT = 0xA8,

        TEST_PS2_CONTROLLER = 0xAA,
        TEST_FIRST_PS2_PORT = 0xAB,

        /// ... (TODO: more commands)
    };

    X86_GenericKeyboardController() {
        port_start = 0x60;
        port_end = 0x66;
        name = "KeyboardController (generic)";
        irq = 0;

        Read8 = &Read8Impl;
        Read16 = &Read16Impl;
        Write8 = &Write8Impl;
        Write16 = &Write16Impl;

    }

    static uint8_t Read8Impl(void* device, uint16_t port);
    static void Write8Impl(void* device, uint16_t port, uint8_t val);

    static uint16_t Read16Impl(void* device, uint16_t port);
    static void Write16Impl(void* device, uint16_t port, uint16_t val);

    uint8_t ram[64];
};

#endif // __DEVICES_KeyboardController_H__
