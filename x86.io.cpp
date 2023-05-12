#include "x86.h"

uint8_t X86_IO_F100_Read8(void *device, uint16_t port) { return 0; }

void X86_IO_F100_Write8(void *device, uint16_t port, uint8_t val) {}

uint16_t X86_IO_F100_Read16(void *device, uint16_t port) { return 0; }

void X86_IO_F100_Write16(void *device, uint16_t port, uint16_t val) {
  printf("%04Xh -> I/O port F100h.\n", val);
}

uint8_t X86_IO_80_Read8(void *device, uint16_t port) { return 0; }

void X86_IO_80_Write8(void *device, uint16_t port, uint8_t val) {
  printf("%02Xh -> I/O port F100h.\n", val);
}

uint16_t X86_IO_80_Read16(void *device, uint16_t port) { return 0; }

void X86_IO_80_Write16(void *device, uint16_t port, uint16_t val) {}
