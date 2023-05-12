#ifndef __X86_IO_H__
#define __X86_IO_H__

#include <cstdint>

uint8_t X86_IO_F100_Read8(void* device, uint16_t port);
void X86_IO_F100_Write8(void* device, uint16_t port, uint8_t val);
uint16_t X86_IO_F100_Read16(void* device, uint16_t port);
void X86_IO_F100_Write16(void* device, uint16_t port, uint16_t val);

#endif // __X86.IO_H__
