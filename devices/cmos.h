#ifndef __DEVICES_CMOS_H__
#define __DEVICES_CMOS_H__

#include "../x86.h"
#include <cstdint>

struct X86_GenericCMOS : public X86_IODeviceBase {
    enum Reg {
        RTC_SECOND = 0,
        RTC_MINUTE = 0x02,
        RTC_HOUR = 0x04,
        RTC_DOW = 0x06,
        RTC_DAY = 0x7,
        RTC_MONTH = 0x8,
        RTC_YEAR = 9,
        RTC_CENTURY = 0x32,
        
    };

    X86_GenericCMOS() {
        port_start = 0x70;
        port_end = 0x72;
        name = "CMOS (generic)";
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
    uint8_t cur_reg = RTC_SECOND;
};

#endif // __DEVICES_CMOS_H__
