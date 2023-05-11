#include "x86.h"

uint16_t X86_MEM_Read16(uint32_t addr) {
  return (X86_MEM_Read8(addr)) | (X86_MEM_Read8(addr + 1) << 8);
}
void X86_MEM_Write16(uint32_t addr, uint16_t val) {
  uint8_t p1 = (val >> 0) & 0xFF;
  uint8_t p2 = (val >> 8) & 0xFF;

  X86_MEM_Write8(addr, p1);
  X86_MEM_Write8(addr + 1, p2);
}

uint32_t X86_MEM_Read32(uint32_t addr) {
  return (X86_MEM_Read16(addr)) | (X86_MEM_Read16(addr + 2) << 16);
}
void X86_MEM_Write32(uint32_t addr, uint32_t val) {
  uint16_t p1 = (val & 0xFFFF0000) >> 16;
  uint16_t p2 = (val & 0x0000FFFF);

  X86_MEM_Write16(addr, p1);
  X86_MEM_Write16(addr + 2, p2);
}

BOOL X86_NOP() { return TRUE; }

BOOL X86_JMP_REL8(uint8_t disp) {
  X86_CPU_gRegs.CS_PLUS_IP = X86_CPU_gRegs.CS_PLUS_IP + disp;
  return TRUE;
}

BOOL X86_MOV_R16_IMM16(uint16_t *dst, uint16_t imm) {
  *dst = imm;
  return TRUE;
}

BOOL X86_MOV_R16_IMM8(uint16_t *dst, uint8_t imm) {
  *dst = imm;
  return TRUE;
}

BOOL X86_MOV_BX_IMM16(uint16_t imm) {
  return X86_MOV_R16_IMM16(&X86_CPU_gRegs.BX, imm);
}

BOOL X86_MOV_AH_IMM8(uint8_t imm) {
  return X86_MOV_R16_IMM8(&X86_CPU_gRegs.AH, imm);
}

BOOL X86_MOV_AL_IMM8(uint8_t imm) {
  return X86_MOV_R16_IMM8(&X86_CPU_gRegs.AL, imm);
}

BOOL X86_MOV_SREG_RM16(X86_CPU_MODRM modrm) {
  switch (modrm) {
  case 0xDB:
    X86_MOV_R16_IMM16(&X86_CPU_gRegs.DS, X86_CPU_gRegs.BX);
    break;
  default:
    printf("Unknown modrm for ``MOV Sreg,r/m16**``: %02Xh!\n", modrm);
    return FALSE;
  }
  return TRUE;
}

BOOL X86_MOV_AX_MOFFS16(uint16_t offset) {
  return X86_MOV_R16_IMM16(&X86_CPU_gRegs.AX, X86_MEM_Read16(X86_CPU_SEGOFF(
                                                  X86_CPU_gRegs.DS, offset)));
}

BOOL X86_JMP_RM16(uint8_t modrm) {
  switch (modrm) {
  case 0xE0: { // jmp ax
    X86_CPU_gRegs.CS_PLUS_IP =
        X86_CPU_SEGOFF(X86_CPU_gRegs.DS, X86_CPU_gRegs.AX);
  } break;
  default:
    printf("Unknown modrm for X86_JMP_RM16: %02Xh!\n", modrm);
    return FALSE;
  }
  return TRUE;
}

BOOL X86_LONGJUMP_16(uint32_t operand) {
  uint16_t target_cs = (operand & 0xFFFF0000) >> 16;
  uint16_t target_ip = (operand & 0x0000FFFF);

  X86_CPU_gRegs.CS_PLUS_IP = X86_CPU_SEGOFF(target_cs, target_ip);
  return TRUE;
}

BOOL X86_MOV_MOFFS16_AX(uint16_t offs) {
    X86_MEM_Write16(X86_CPU_SEGOFF(X86_CPU_gRegs.DS, offs), X86_CPU_gRegs.AX);
    return TRUE;
}

X86_CPU_InstructionDef X86_CPU_gInstrMap[] = {
    [0x90] = {.disas = "NOP", .num_extra_bytes = 0, .execute = X86_NOP},
    [0xEB] = {.disas = "JMP 0x%02X",
              .num_extra_bytes = 1,
              .execute = X86_JMP_REL8},
    [0xB0] = {.disas = "MOV AL, 0x%02X",
              .num_extra_bytes = 1,
              .execute = X86_MOV_AL_IMM8},
    [0xB4] = {.disas = "MOV AH, 0x%02X",
              .num_extra_bytes = 1,
              .execute = X86_MOV_AH_IMM8},
    [0xBB] = {.disas = "MOV BX, 0x%04X",
              .num_extra_bytes = 2,
              .execute = X86_MOV_BX_IMM16},
    [0x8E] = {.disas = "MOV %s, %s",
              .num_extra_bytes = 1,
              .execute = X86_MOV_SREG_RM16},
    [0xA1] = {.disas = "MOV AX, 0x%04X",
              .num_extra_bytes = 2, // TODO: in protected mode it's 4!
              .execute = X86_MOV_AX_MOFFS16},
    [0xA3] = {.disas = "MOV [0x%04X], AX",
              .num_extra_bytes = 2, // TODO: in protected mode it's 4!
              .execute = X86_MOV_MOFFS16_AX},
    [0xEA] = {.disas = "LJMP %s:%s",
              .num_extra_bytes = 4,
              .execute = X86_LONGJUMP_16},
    [0xFF] = {.disas = "JMP %s", .num_extra_bytes = 1, .execute = X86_JMP_RM16},
};

X86_CPU_Registers X86_CPU_gRegs = {};
BOOL X86_CPU_Step(void) {
  uint8_t op = X86_MEM_Read8(X86_CPU_gRegs.CS_PLUS_IP++);
  if (!X86_CPU_gInstrMap[op].execute) {
    printf("ERROR: UNIMPLEMENTED OPCODE %02Xh at %08Xh\n", op,
           X86_CPU_gRegs.CS_PLUS_IP - 1);
    return FALSE;
  }
  BOOL succ = TRUE;
  uint32_t operand;
  if (X86_CPU_gInstrMap[op].num_extra_bytes > 0) {
    switch (X86_CPU_gInstrMap[op].num_extra_bytes) {
    case 1:
      succ = ((X86_CPU_InstrOp8)X86_CPU_gInstrMap[op].execute)(
          X86_MEM_Read8(X86_CPU_gRegs.CS_PLUS_IP++));
      break;
    case 2: {
      operand = X86_MEM_Read16(X86_CPU_gRegs.CS_PLUS_IP);
      X86_CPU_gRegs.CS_PLUS_IP += 2;
      succ = ((X86_CPU_InstrOp16)X86_CPU_gInstrMap[op].execute)(operand);
      break;
    }
    case 4: {
      operand = X86_MEM_Read32(X86_CPU_gRegs.CS_PLUS_IP);
      X86_CPU_gRegs.CS_PLUS_IP += 4;
      succ = ((X86_CPU_InstrOp32)X86_CPU_gInstrMap[op].execute)(operand);
      break;
    }
    default: {
      return FALSE; // ???
    }
    }
  } else
    succ = ((X86_CPU_InstrVoid)X86_CPU_gInstrMap[op].execute)();
  return succ;
}
