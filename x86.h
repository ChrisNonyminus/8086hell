

#ifndef __X86_H__
#define __X86_H__

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define BOOL int
#define TRUE 1
#define FALSE 0

typedef BOOL (*X86_CPU_InstrVoid)(void);
typedef BOOL (*X86_CPU_InstrOp8)(uint8_t);
typedef BOOL (*X86_CPU_InstrOp16)(uint16_t);
typedef BOOL (*X86_CPU_InstrOp32)(uint32_t);

typedef struct {
  const char *disas;
  int num_extra_bytes;
  void *execute;
} X86_CPU_InstructionDef;

typedef uint8_t X86_CPU_MODRM;

#define X86_CPU_MODRM_GET_MOD(modrm) ((modrm & 0b11000000) >> 6)
#define X86_CPU_MODRM_GET_REG(modrm) ((modrm & 0b00111000) >> 3)
#define X86_CPU_MODRM_GET_R_M(modrm) ((modrm & 0b00000111))

#define X86_CPU_REG_AX 0b000
#define X86_CPU_REG_CX 0b001
#define X86_CPU_REG_DX 0b010
#define X86_CPU_REG_BX 0b011
#define X86_CPU_REG_SP 0b100
#define X86_CPU_REG_BP 0b101
#define X86_CPU_REG_SI 0b110
#define X86_CPU_REG_DI 0b111

#define X86_CPU_SREG_ES 0b00
#define X86_CPU_SREG_CS 0b01
#define X86_CPU_SREG_SS 0b10
#define X86_CPU_SREG_DS 0b11

extern X86_CPU_InstructionDef X86_CPU_gInstrMap[];

typedef struct {
  uint32_t CS_PLUS_IP;
  union {
    uint32_t EBX;
    uint16_t BX;
  };
  union {
    uint32_t EAX;
    uint16_t AX;
  };
  uint16_t AH;
  uint16_t AL;
  uint16_t DS;
  uint16_t CS;
} X86_CPU_Registers;
extern X86_CPU_Registers X86_CPU_gRegs;

BOOL X86_CPU_Step(void);

#define X86_CPU_SEGOFF(seg, off) ((seg << 4) + (off))

/*
X86 System Memory Map (according to OSdev wiki):
00000-A0000: 640 KiB RAM (low memory)
F0000-FFFFF: 64KiB motherboard BIOS
*/
extern uint8_t X86_LLE_MEM_gBios[0x10000];
extern uint8_t X86_LLE_MEM_gLoMem[0x000A0000];

uint8_t X86_LLE_MEM_Read8(uint32_t addr);
void X86_LLE_MEM_Write8(uint32_t addr, uint8_t val);

BOOL X86_LLE_BIOS_Load(const char *filename);

#ifdef X86_HLE
#define X86_MEM_Read8  // TODO
#define X86_MEM_Write8 // TODO
#else
#define X86_MEM_Read8 X86_LLE_MEM_Read8
#define X86_MEM_Write8 X86_LLE_MEM_Write8
#endif

uint16_t X86_MEM_Read16(uint32_t addr);
void X86_MEM_Write16(uint32_t addr, uint16_t val);
uint32_t X86_MEM_Read32(uint32_t addr);
void X86_MEM_Write32(uint32_t addr, uint32_t val);

#ifdef __cplusplus
}
#endif

#endif // __X86_H__
