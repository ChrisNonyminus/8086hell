

#ifndef __X86_H__
#define __X86_H__

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef int BOOL;
#define TRUE 1
#define FALSE 0

typedef BOOL (*X86_CPU_InstrFunc)(void);

typedef enum {
  X86_OP_IN_REG = (1 << 1),
  X86_OP_IN_MEM = (1 << 2),
  X86_OP_OUT_REG = (1 << 3),
  X86_OP_OUT_MEM = (1 << 4),
  X86_OP_IN_IMM = (1 << 5),
  X86_OP_MODRM = (1 << 6),
  // X86_OP_REG = (1 << 7),
} X86_CPU_OPERAND_TYPE;

typedef struct {
  uint8_t opcode;
  const char *name;
  X86_CPU_InstrFunc execute16;
  X86_CPU_InstrFunc execute32;
  uint8_t operand_types;
} X86_CPU_InstructionDef;

#define X86_CPU_INSDEF_B0(reg)                                                 \
  {                                                                            \
    0xB0 + X86_CPU_W0REG_##reg, "MOV", X86_MOV_##reg##_IMM8, NULL,             \
        X86_OP_IN_IMM                                                          \
  }
#define X86_CPU_INSDEF_B8(reg)                                                 \
  {                                                                            \
    0xB8 + X86_CPU_REG_##reg, "MOV", X86_MOV_##reg##_IMM16, NULL,              \
        X86_OP_IN_IMM                                                          \
  }

typedef uint8_t X86_CPU_MODRM;

#define X86_CPU_MODRM_GET_MOD(modrm) ((modrm & 0b11000000) >> 6)
#define X86_CPU_MODRM_GET_REG1(modrm) ((modrm & 0b00111000) >> 3)
#define X86_CPU_MODRM_GET_REG2(modrm) ((modrm & 0b00000111))

#define X86_CPU_REG_AX 0b000
#define X86_CPU_REG_CX 0b001
#define X86_CPU_REG_DX 0b010
#define X86_CPU_REG_BX 0b011
#define X86_CPU_REG_SP 0b100
#define X86_CPU_REG_BP 0b101
#define X86_CPU_REG_SI 0b110
#define X86_CPU_REG_DI 0b111

#define X86_CPU_W0REG_AL 0b000
#define X86_CPU_W0REG_CL 0b001
#define X86_CPU_W0REG_DL 0b010
#define X86_CPU_W0REG_BL 0b011
#define X86_CPU_W0REG_AH 0b100
#define X86_CPU_W0REG_CH 0b101
#define X86_CPU_W0REG_DH 0b110
#define X86_CPU_W0REG_BH 0b111

#define X86_CPU_SREG_ES 0b00
#define X86_CPU_SREG_CS 0b01
#define X86_CPU_SREG_SS 0b10
#define X86_CPU_SREG_DS 0b11

extern X86_CPU_InstructionDef X86_CPU_gInstrMap[];

typedef struct {
  union {
    uint32_t dword;
    struct {
      union {
        uint16_t word;
        struct {
          uint8_t l;
          uint8_t h;
        };
      };
      uint16_t hiword;
    };
  };
} X86_CPU_Register;

typedef struct {
  X86_CPU_Register EIP;
  X86_CPU_Register EAX;
  X86_CPU_Register EBX;
  X86_CPU_Register ECX;
  X86_CPU_Register EDX;
  X86_CPU_Register ESP;
  X86_CPU_Register EBP;
  X86_CPU_Register ESI;
  X86_CPU_Register EDI;
  uint16_t ES;
  uint16_t CS;
  uint16_t SS;
  uint16_t DS;
} X86_CPU_Registers;
extern X86_CPU_Registers X86_CPU_gRegs;

typedef enum { X86_MACHINE_80186 } X86_EMU_MachineType;
typedef struct {
  uint16_t port_start;
  uint16_t port_end;
  uint8_t (*Read8)(void *device);
  void (*Write8)(void *device, uint8_t val);
  uint16_t (*Read16)(void *device);
  void (*Write16)(void *device, uint16_t val);
} X86_IO_Handler;
typedef struct {
  X86_EMU_MachineType type;
  X86_IO_Handler *io_handlers;
  size_t num_io_handlers;
  void (*Deinit)(void);
  uint8_t (*IO_Read8)(uint16_t port);
  void (*IO_Write8)(uint16_t port, uint8_t val);
  uint16_t (*IO_Read16)(uint16_t port);
  void (*IO_Write16)(uint16_t port, uint16_t val);
} X86_EMU_Machine;
extern X86_EMU_Machine *X86_EMU_gActiveMachine;

BOOL X86_EMU_InitMachine(X86_EMU_MachineType type);
BOOL X86_EMU_FreeMachine(void);

BOOL X86_CPU_Step(void);

#define X86_CPU_SEGOFF(seg, off) ((seg << 4) + (off))
#define X86_CPU_SEG(segoff) ((segoff & 0xFFFF0000) >> 4)
#define X86_CPU_OFF(segoff) (segoff & 0xFFFF)

/*
X86 System Memory Map (according to OSdev wiki):
00000-A0000: 640 KiB RAM (low memory)
F0000-FFFFF: 64KiB motherboard BIOS
*/
extern uint8_t X86_LLE_MEM_gBios[0x10000];
extern uint8_t X86_LLE_MEM_gLoMem[0xA0000];

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
