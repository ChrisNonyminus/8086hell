#include "x86.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t X86_LLE_MEM_gBios[0x10000] = {0};
uint8_t X86_LLE_MEM_gLoMem[0x000A0000] = {0};

int X86_LLE_BIOS_Load(const char *filename) {
  FILE *biosFile = fopen(filename, "rb");

  if (!biosFile) {
    return 0;
  }

  fseek(biosFile, 0, SEEK_END);
  size_t fsize = ftell(biosFile);

  if ((fsize / 1024) != 64) {
    printf("BIOS filesize wasn't 64KB!\n");
    printf("Expected 64KB, got: %.2fKB\n", (float)fsize / 1024.0f);
    return 0;
  }

  rewind(biosFile);

  fread(X86_LLE_MEM_gBios, 1, fsize, biosFile);

  X86_CPU_gRegs.EIP.word = 0; // ensure we start execution on the reset vector
  X86_CPU_gRegs.CS = 0xFFFF; // ensure we start execution on the reset vector

  return 1;
}

uint8_t X86_LLE_MEM_Read8(uint32_t addr) {
  if (addr >= 0xF0000 && addr <= 0xFFFFF) {
    // BIOS
    return X86_LLE_MEM_gBios[addr - 0xF0000];
  } else if (addr >= 0 && addr <= 0x0009FFFF) {
    return X86_LLE_MEM_gLoMem[addr];
  }

  printf(" READ8: UNMAPPED ADDRESS!\n");
  printf("%#08x\n", addr);
  exit(-1);
}

void X86_LLE_MEM_Write8(uint32_t addr, uint8_t val) {
  if (addr >= 0xF0000 && addr <= 0xFFFFF) {
    // BIOS
    X86_LLE_MEM_gBios[addr - 0xF0000] = val;
  } else if (addr >= 0 && addr <= 0x0009FFFF) {
    X86_LLE_MEM_gLoMem[addr] = val;
  } else {

    printf("WRITE8: UNMAPPED ADDRESS!\n");
    printf("%#08x\n", addr);
    exit(-1);
  }
}
