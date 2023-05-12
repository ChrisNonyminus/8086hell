#include "cmos.h"
#include <cstdint>

uint8_t X86_GenericCMOS::Read8Impl(void *device, uint16_t port) {
  X86_GenericCMOS *cmos = static_cast<X86_GenericCMOS *>(device);
  switch (port) {
  case 0x70:
    return cmos->cur_reg;
  case 0x71:
    return cmos->ram[cmos->cur_reg];
  default:
    printf("WARNING: Bad CMOS port %04Xh!\n", port);
    return -1;
  }
}

void X86_GenericCMOS::Write8Impl(void *device, uint16_t port, uint8_t val) {
  X86_GenericCMOS *cmos = static_cast<X86_GenericCMOS *>(device);
  switch (port) {
  case 0x70:
    cmos->cur_reg = val & 0x7f;
    return;
  case 0x71:
    cmos->ram[cmos->cur_reg] = val;
    return;
  default:
    break;
  }
  printf("WARNING: Bad CMOS port %04Xh!\n", port);
}

uint16_t X86_GenericCMOS::Read16Impl(void *device, uint16_t port) {
  X86_GenericCMOS *cmos = static_cast<X86_GenericCMOS *>(device);
  printf("WARNING: You shouldn't do a Read16 on the CMOS, apparently.\n");
  return -1;
}

void X86_GenericCMOS::Write16Impl(void *device, uint16_t port, uint16_t val) {
  X86_GenericCMOS *cmos = static_cast<X86_GenericCMOS *>(device);
  printf("WARNING: You shouldn't do a Write16 on the CMOS, apparently.\n");
}

