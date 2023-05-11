#include "x86.h"
#include <stdlib.h>

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

BOOL X86_JMP_REL8(void) {
  uint8_t disp =
      X86_MEM_Read8(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++));
  X86_CPU_gRegs.EIP.word = X86_CPU_gRegs.EIP.word + disp;
  return TRUE;
}

BOOL X86_MOV_R16_IMM16(uint16_t *dst, uint16_t imm) {
  *dst = imm;
  return TRUE;
}

BOOL X86_MOV_R8_IMM8(uint8_t *dst, uint8_t imm) {
  *dst = imm;
  return TRUE;
}

BOOL X86_MOV_BX_IMM16(void) {
  uint16_t imm =
      X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
  X86_CPU_gRegs.EIP.word += 2;
  return X86_MOV_R16_IMM16(&X86_CPU_gRegs.EBX.word, imm);
}
BOOL X86_MOV_BP_IMM16(void) {
  uint16_t imm =
      X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
  X86_CPU_gRegs.EIP.word += 2;
  return X86_MOV_R16_IMM16(&X86_CPU_gRegs.EBP.word, imm);
}
BOOL X86_MOV_SP_IMM16(void) {
  uint16_t imm =
      X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
  X86_CPU_gRegs.EIP.word += 2;
  return X86_MOV_R16_IMM16(&X86_CPU_gRegs.ESP.word, imm);
}
BOOL X86_MOV_SI_IMM16(void) {
  uint16_t imm =
      X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
  X86_CPU_gRegs.EIP.word += 2;
  return X86_MOV_R16_IMM16(&X86_CPU_gRegs.ESI.word, imm);
}
BOOL X86_MOV_DI_IMM16(void) {
  uint16_t imm =
      X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
  X86_CPU_gRegs.EIP.word += 2;
  return X86_MOV_R16_IMM16(&X86_CPU_gRegs.EDI.word, imm);
}
BOOL X86_MOV_AX_IMM16(void) {
  uint16_t imm =
      X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
  X86_CPU_gRegs.EIP.word += 2;
  return X86_MOV_R16_IMM16(&X86_CPU_gRegs.EAX.word, imm);
}
BOOL X86_MOV_CX_IMM16(void) {
  uint16_t imm =
      X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
  X86_CPU_gRegs.EIP.word += 2;
  return X86_MOV_R16_IMM16(&X86_CPU_gRegs.ECX.word, imm);
}

BOOL X86_MOV_DX_IMM16(void) {
  uint16_t imm =
      X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
  X86_CPU_gRegs.EIP.word += 2;
  return X86_MOV_R16_IMM16(&X86_CPU_gRegs.EDX.word, imm);
}

BOOL X86_MOV_AH_IMM8(void) {
  return X86_MOV_R8_IMM8(&X86_CPU_gRegs.EAX.h,
                         X86_MEM_Read8(X86_CPU_SEGOFF(
                             X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++)));
}

BOOL X86_MOV_AL_IMM8(void) {
  return X86_MOV_R8_IMM8(&X86_CPU_gRegs.EAX.l,
                         X86_MEM_Read8(X86_CPU_SEGOFF(
                             X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++)));
}

BOOL X86_MOV_BH_IMM8(void) {
  return X86_MOV_R8_IMM8(&X86_CPU_gRegs.EBX.h,
                         X86_MEM_Read8(X86_CPU_SEGOFF(
                             X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++)));
}

BOOL X86_MOV_BL_IMM8(void) {
  return X86_MOV_R8_IMM8(&X86_CPU_gRegs.EBX.l,
                         X86_MEM_Read8(X86_CPU_SEGOFF(
                             X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++)));
}

BOOL X86_MOV_CH_IMM8(void) {
  return X86_MOV_R8_IMM8(&X86_CPU_gRegs.ECX.h,
                         X86_MEM_Read8(X86_CPU_SEGOFF(
                             X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++)));
}

BOOL X86_MOV_CL_IMM8(void) {
  return X86_MOV_R8_IMM8(&X86_CPU_gRegs.ECX.l,
                         X86_MEM_Read8(X86_CPU_SEGOFF(
                             X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++)));
}

BOOL X86_MOV_DH_IMM8(void) {
  return X86_MOV_R8_IMM8(&X86_CPU_gRegs.EDX.h,
                         X86_MEM_Read8(X86_CPU_SEGOFF(
                             X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++)));
}

BOOL X86_MOV_DL_IMM8(void) {
  return X86_MOV_R8_IMM8(&X86_CPU_gRegs.EDX.l,
                         X86_MEM_Read8(X86_CPU_SEGOFF(
                             X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++)));
}

BOOL X86_MOV_R8_RM8(void) {
  uint8_t modrm =
      X86_MEM_Read8(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++));
  uint8_t *dst;
  uint8_t src;
  switch (X86_CPU_MODRM_GET_REG1(modrm)) {
  case X86_CPU_W0REG_AL:
    dst = &X86_CPU_gRegs.EAX.l;
    break;
  case X86_CPU_W0REG_AH:
    dst = &X86_CPU_gRegs.EAX.h;
    break;
  case X86_CPU_W0REG_CL:
    dst = &X86_CPU_gRegs.ECX.l;
    break;
  case X86_CPU_W0REG_CH:
    dst = &X86_CPU_gRegs.ECX.h;
    break;
  case X86_CPU_W0REG_DL:
    dst = &X86_CPU_gRegs.EDX.l;
    break;
  case X86_CPU_W0REG_DH:
    dst = &X86_CPU_gRegs.EDX.h;
    break;
  case X86_CPU_W0REG_BL:
    dst = &X86_CPU_gRegs.EBX.l;
    break;
  case X86_CPU_W0REG_BH:
    dst = &X86_CPU_gRegs.EBX.h;
    break;
  default:
    printf("UNHANDLED REG: %d\n", X86_CPU_MODRM_GET_REG1(modrm));
    return FALSE;
  }
  if (X86_CPU_MODRM_GET_MOD(modrm) == 0b11) {
    switch (X86_CPU_MODRM_GET_REG2(modrm)) {
    case X86_CPU_W0REG_AL:
      src = X86_CPU_gRegs.EAX.l;
      break;
    case X86_CPU_W0REG_AH:
      src = X86_CPU_gRegs.EAX.h;
      break;
    case X86_CPU_W0REG_CL:
      src = X86_CPU_gRegs.ECX.l;
      break;
    case X86_CPU_W0REG_CH:
      src = X86_CPU_gRegs.ECX.h;
      break;
    case X86_CPU_W0REG_DL:
      src = X86_CPU_gRegs.EDX.l;
      break;
    case X86_CPU_W0REG_DH:
      src = X86_CPU_gRegs.EDX.h;
      break;
    case X86_CPU_W0REG_BL:
      src = X86_CPU_gRegs.EBX.l;
      break;
    case X86_CPU_W0REG_BH:
      src = X86_CPU_gRegs.EBX.h;
      break;
    default:
      printf("UNHANDLED REG: %d\n", X86_CPU_MODRM_GET_REG2(modrm));
      return FALSE;
    }
    return X86_MOV_R8_IMM8(dst, src);
  } else {
    if (X86_CPU_MODRM_GET_MOD(modrm) == 0 &&
        X86_CPU_MODRM_GET_REG2(modrm) == 0b110) {
      uint16_t offs = X86_MEM_Read16(
          X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
      X86_CPU_gRegs.EIP.word += 2;
      return X86_MOV_R8_IMM8(
          dst, X86_MEM_Read8(X86_CPU_SEGOFF(X86_CPU_gRegs.DS, offs)));
    }
  }
  printf("UNHANDLED MODRM: %02Xh\n", (modrm));
  return FALSE;
}

BOOL X86_MOV_R16_RM16(void) {
  uint8_t modrm =
      X86_MEM_Read8(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++));
  uint16_t *dst;
  uint16_t src;
  switch (X86_CPU_MODRM_GET_REG2(modrm)) {
  case X86_CPU_REG_AX:
    dst = &X86_CPU_gRegs.EAX.word;
    break;
  case X86_CPU_REG_CX:
    dst = &X86_CPU_gRegs.ECX.word;
    break;
  case X86_CPU_REG_DX:
    dst = &X86_CPU_gRegs.EDX.word;
    break;
  case X86_CPU_REG_BX:
    dst = &X86_CPU_gRegs.EBX.word;
    break;
  case X86_CPU_REG_SP:
    dst = &X86_CPU_gRegs.ESP.word;
    break;
  case X86_CPU_REG_BP:
    dst = &X86_CPU_gRegs.EBP.word;
    break;
  case X86_CPU_REG_SI:
    dst = &X86_CPU_gRegs.ESI.word;
    break;
  case X86_CPU_REG_DI:
    dst = &X86_CPU_gRegs.EDI.word;
    break;
  default:
    printf("UNHANDLED REG: %d\n", X86_CPU_MODRM_GET_REG2(modrm));
    return FALSE;
  }
  if (X86_CPU_MODRM_GET_MOD(modrm) == 0b11) {
    switch (X86_CPU_MODRM_GET_REG1(modrm)) {
    case X86_CPU_REG_AX:
      src = X86_CPU_gRegs.EAX.word;
      break;
    case X86_CPU_REG_CX:
      src = X86_CPU_gRegs.ECX.word;
      break;
    case X86_CPU_REG_DX:
      src = X86_CPU_gRegs.EDX.word;
      break;
    case X86_CPU_REG_BX:
      src = X86_CPU_gRegs.EBX.word;
      break;
    case X86_CPU_REG_SP:
      src = X86_CPU_gRegs.ESP.word;
      break;
    case X86_CPU_REG_BP:
      src = X86_CPU_gRegs.EBP.word;
      break;
    case X86_CPU_REG_SI:
      src = X86_CPU_gRegs.ESI.word;
      break;
    case X86_CPU_REG_DI:
      src = X86_CPU_gRegs.EDI.word;
      break;
    default:
      printf("UNHANDLED REG: %d\n", X86_CPU_MODRM_GET_REG1(modrm));
      return FALSE;
    }
    return X86_MOV_R16_IMM16(dst, src);
  } else {
    if (X86_CPU_MODRM_GET_MOD(modrm) == 0) {
      if (X86_CPU_MODRM_GET_REG2(modrm) == 0b110) {
        src = X86_MEM_Read16(
            X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
        X86_CPU_gRegs.EIP.word += 2;
        return X86_MOV_R16_IMM16(
            dst, X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.DS, src)));
      } else if (X86_CPU_MODRM_GET_REG2(modrm) == 0b111) {
        src = X86_CPU_gRegs.EAX.word;
        X86_MEM_Write16(X86_CPU_SEGOFF(X86_CPU_gRegs.DS, X86_CPU_gRegs.EBX.word), src);
        return TRUE;
      }
    }
  }
  printf("UNHANDLED MODRM: %02Xh\n", (modrm));
  return FALSE;
}

// FIXME
BOOL X86_MOV_SREG_RM16(void) {
  uint8_t modrm =
      X86_MEM_Read8(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++));
  uint16_t *dst;
  uint16_t src;
  switch (X86_CPU_MODRM_GET_REG1(modrm)) {
  case X86_CPU_SREG_ES:
    dst = &X86_CPU_gRegs.ES;
    break;
  case X86_CPU_SREG_CS:
    dst = &X86_CPU_gRegs.CS;
    break;
  case X86_CPU_SREG_SS:
    dst = &X86_CPU_gRegs.SS;
    break;
  case X86_CPU_SREG_DS:
    dst = &X86_CPU_gRegs.DS;
    break;
  default:
    printf("UNHANDLED SREG: %d\n", X86_CPU_MODRM_GET_REG1(modrm));
    return FALSE;
  }
  if (X86_CPU_MODRM_GET_MOD(modrm) == 0b11) {
    switch (X86_CPU_MODRM_GET_REG2(modrm)) {
    case X86_CPU_REG_AX:
      src = X86_CPU_gRegs.EAX.word;
      break;
    case X86_CPU_REG_CX:
      src = X86_CPU_gRegs.ECX.word;
      break;
    case X86_CPU_REG_DX:
      src = X86_CPU_gRegs.EDX.word;
      break;
    case X86_CPU_REG_BX:
      src = X86_CPU_gRegs.EBX.word;
      break;
    case X86_CPU_REG_SP:
      src = X86_CPU_gRegs.ESP.word;
      break;
    case X86_CPU_REG_BP:
      src = X86_CPU_gRegs.EBP.word;
      break;
    case X86_CPU_REG_SI:
      src = X86_CPU_gRegs.ESI.word;
      break;
    case X86_CPU_REG_DI:
      src = X86_CPU_gRegs.EDI.word;
      break;
    default:
      printf("UNHANDLED REG: %d\n", X86_CPU_MODRM_GET_REG2(modrm));
      return FALSE;
    }
    return X86_MOV_R16_IMM16(dst, src);
  } else {
    if (X86_CPU_MODRM_GET_MOD(modrm) == 0 &&
        X86_CPU_MODRM_GET_REG2(modrm) == 0b110) {
      src = X86_MEM_Read16(
          X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
      X86_CPU_gRegs.EIP.word += 2;
      return X86_MOV_R16_IMM16(
          dst, X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.DS, src)));
    }
  }
  printf("UNHANDLED MODRM: %02Xh\n", (modrm));
  return FALSE;
}

BOOL X86_MOV_AX_MOFFS16(void) {
  uint16_t offset =
      X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
  X86_CPU_gRegs.EIP.word += 2;
  return X86_MOV_R16_IMM16(
      &X86_CPU_gRegs.EAX.word,
      X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.DS, offset)));
}

BOOL X86_JMP_RM16(void) {
  uint8_t modrm =
      X86_MEM_Read8(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++));
  if (X86_CPU_MODRM_GET_MOD(modrm) == 0b11 &&
      X86_CPU_MODRM_GET_REG1(modrm) == 0b100) {
    switch (X86_CPU_MODRM_GET_REG2(modrm)) {
    case X86_CPU_REG_AX: { // jmp ax
      X86_CPU_gRegs.EIP.word = X86_CPU_gRegs.EAX.word;
      return TRUE;
    } break;
    case X86_CPU_REG_CX: { // jmp cx
      X86_CPU_gRegs.EIP.word = X86_CPU_gRegs.ECX.word;
      return TRUE;
    } break;
    case X86_CPU_REG_BX: { // jmp bx
      X86_CPU_gRegs.EIP.word = X86_CPU_gRegs.EBX.word;
      return TRUE;
    } break;
    case X86_CPU_REG_DX: { // jmp cx
      X86_CPU_gRegs.EIP.word = X86_CPU_gRegs.EDX.word;
      return TRUE;
    } break;
    default:
      break;
    }
  }
  if (X86_CPU_MODRM_GET_MOD(modrm) == 0b01 &&
      X86_CPU_MODRM_GET_REG2(modrm) == 0b011) {
    int8_t disp8 = X86_MEM_Read8(
        X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++));
    uint16_t offs = X86_CPU_gRegs.EBP.word + X86_CPU_gRegs.EDI.word + disp8;
    X86_CPU_gRegs.EIP.word =
        X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.SS, offs));
    return TRUE;
  }
  printf("Unknown modrm for X86_JMP_RM16: %02Xh!\n", modrm);
  return FALSE;
}

BOOL X86_LONGJUMP_16(void) {
  uint16_t target_ip =
      X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
  X86_CPU_gRegs.EIP.word += 2;
  uint16_t target_cs =
      X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
  X86_CPU_gRegs.EIP.word += 2;
  X86_CPU_gRegs.CS = target_cs;

  X86_CPU_gRegs.EIP.word = target_ip;
  return TRUE;
}

BOOL X86_MOV_RM16_IMM16(void) {
  uint8_t modrm =
      X86_MEM_Read8(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++));
  switch (modrm) {
  case 6: {
    uint16_t offs = X86_MEM_Read16(
        X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
    X86_CPU_gRegs.EIP.word += 2;
    uint16_t val = X86_MEM_Read16(
        X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
    X86_CPU_gRegs.EIP.word += 2;
    X86_MEM_Write16(X86_CPU_SEGOFF(X86_CPU_gRegs.DS, offs), val);
    return TRUE;
  }
  case 0xC0: {
    uint16_t val = X86_MEM_Read16(
        X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
    X86_CPU_gRegs.EIP.word += 2;
    X86_CPU_gRegs.EAX.word = val;
    return TRUE;
  }
  default:
    break;
  }
  printf("Unknown modrm for X86_MOV_RM16_IMM16: %02Xh!\n", modrm);
  return FALSE;
}

BOOL X86_MOV_MOFFS16_AX(void) {
  uint16_t offs =
      X86_MEM_Read16(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
  X86_CPU_gRegs.EIP.word += 2;
  X86_MEM_Write16(X86_CPU_SEGOFF(X86_CPU_gRegs.DS, offs),
                  X86_CPU_gRegs.EAX.word);
  return TRUE;
}

BOOL X86_MOV_RM16_SREG(void) {
  uint8_t modrm =
      X86_MEM_Read8(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++));
  uint16_t dst;
  uint16_t src;
  switch (X86_CPU_MODRM_GET_REG1(modrm)) {
  case X86_CPU_SREG_ES:
    src = X86_CPU_gRegs.ES;
    break;
  case X86_CPU_SREG_CS:
    src = X86_CPU_gRegs.CS;
    break;
  case X86_CPU_SREG_SS:
    src = X86_CPU_gRegs.SS;
    break;
  case X86_CPU_SREG_DS:
    src = X86_CPU_gRegs.DS;
    break;
  default:
    printf("UNHANDLED SREG: %d\n", X86_CPU_MODRM_GET_REG1(modrm));
    return FALSE;
  }
  if (X86_CPU_MODRM_GET_MOD(modrm) == 0b11) {
    // TODO
  } else {
    if (X86_CPU_MODRM_GET_MOD(modrm) == 0 &&
        X86_CPU_MODRM_GET_REG2(modrm) == 0b110) {
      dst = X86_MEM_Read16(
          X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
      X86_CPU_gRegs.EIP.word += 2;
      X86_MEM_Write16(X86_CPU_SEGOFF(X86_CPU_gRegs.DS, dst), src);
      return TRUE;
    }
  }
  printf("Unknown modrm for X86_MOV_RM16_IMM16: %02Xh!\n", modrm);
  return FALSE;
}

BOOL X86_OUT_DX_AX(void) {
  uint16_t io_port = X86_CPU_gRegs.EDX.word;
  X86_EMU_gActiveMachine->IO_Write16(io_port, X86_CPU_gRegs.EAX.word);
  return TRUE;
}

int X86_CPU_InstructionCmp(void const *lhs, void const *rhs) {
  X86_CPU_InstructionDef const *const l =
      static_cast<X86_CPU_InstructionDef const *const>(lhs);
  X86_CPU_InstructionDef const *const r =
      static_cast<X86_CPU_InstructionDef const *const>(rhs);

  if (l->opcode < r->opcode)
    return -1;
  else if (l->opcode > r->opcode)
    return 1;
  else {
    return 0;
  }
}

X86_CPU_InstructionDef X86_CPU_gInstrMap[] = {
    {0x89, "MOV", X86_MOV_R16_RM16, NULL, X86_OP_MODRM},
    {0x8A, "MOV", X86_MOV_R8_RM8, NULL, X86_OP_MODRM},
    {0x8C, "MOV", X86_MOV_RM16_SREG, NULL, X86_OP_MODRM},
    {0x8E, "MOV", X86_MOV_SREG_RM16, NULL, X86_OP_MODRM},
    {0x90, "NOP", X86_NOP, X86_NOP, 0},

    {0xA1, "MOV", X86_MOV_AX_MOFFS16, NULL, X86_OP_IN_MEM | X86_OP_OUT_REG},
    {0xA3, "MOV", X86_MOV_MOFFS16_AX, NULL, X86_OP_OUT_MEM | X86_OP_IN_REG},
    X86_CPU_INSDEF_B0(AL),
    X86_CPU_INSDEF_B0(CL),
    X86_CPU_INSDEF_B0(DL),
    X86_CPU_INSDEF_B0(BL),
    X86_CPU_INSDEF_B0(AH),
    X86_CPU_INSDEF_B0(CH),
    X86_CPU_INSDEF_B0(DH),
    X86_CPU_INSDEF_B0(BH),

    X86_CPU_INSDEF_B8(AX),
    X86_CPU_INSDEF_B8(CX),
    X86_CPU_INSDEF_B8(DX),
    X86_CPU_INSDEF_B8(BX),
    X86_CPU_INSDEF_B8(SP),
    X86_CPU_INSDEF_B8(BP),
    X86_CPU_INSDEF_B8(SI),
    X86_CPU_INSDEF_B8(DI),
    {0xC7, "MOV", X86_MOV_RM16_IMM16, NULL, X86_OP_MODRM | X86_OP_IN_IMM},
    {0xEA, "JMP FAR", X86_LONGJUMP_16, NULL, X86_OP_IN_IMM},
    {0xEB, "JMP", X86_JMP_REL8, NULL, X86_OP_IN_IMM},
    {0xEF, "OUT", X86_OUT_DX_AX, NULL, 0},
    {0xFF, "JMP FAR", X86_JMP_RM16, NULL, X86_OP_MODRM},
};

void X86_CPU_PrintRegisters() {
  printf("\tPC: %08Xh\n",
         X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word));
  printf("\tEAX: %08Xh\n", X86_CPU_gRegs.EAX.dword);
  printf("\tEBX: %08Xh\n", X86_CPU_gRegs.EBX.dword);
  printf("\tECX: %08Xh\n", X86_CPU_gRegs.ECX.dword);
  printf("\tEDX: %08Xh\n", X86_CPU_gRegs.EDX.dword);
  printf("\tEBP: %08Xh\n", X86_CPU_gRegs.EBP.dword);
  printf("\tEDI: %08Xh\n", X86_CPU_gRegs.EDI.dword);
  printf("\tCS: %08Xh\n", X86_CPU_gRegs.CS);
  printf("\tDS: %08Xh\n", X86_CPU_gRegs.DS);
  printf("\tSS: %08Xh\n", X86_CPU_gRegs.SS);
}

X86_CPU_Registers X86_CPU_gRegs = {};
BOOL X86_CPU_Step(void) {
  printf("CPU STEP!\n");
  X86_CPU_PrintRegisters();

  uint8_t op =
      X86_MEM_Read8(X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word++));
  printf("\topcode: %02Xh\n", op);

  X86_CPU_InstructionDef key = {op};
  const X86_CPU_InstructionDef *res =
      static_cast<const X86_CPU_InstructionDef *>(
          bsearch(&key, X86_CPU_gInstrMap,
                  sizeof(X86_CPU_gInstrMap) / sizeof(X86_CPU_gInstrMap[0]),
                  sizeof(X86_CPU_gInstrMap[0]), X86_CPU_InstructionCmp));

  BOOL succ = FALSE;

  if (res) {
    succ = res->execute16();
  } else {
    printf("ERROR: UNIMPLEMENTED OPCODE %02Xh at %08Xh\n", op,
           X86_CPU_SEGOFF(X86_CPU_gRegs.CS, X86_CPU_gRegs.EIP.word) - 1);
  }

  return succ;
}