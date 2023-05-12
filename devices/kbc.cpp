#include "kbc.h"
#include <cstdint>
#include <cstdlib>

uint8_t X86_GenericKeyboardController::Read8Impl(void *device, uint16_t port) {
  X86_GenericKeyboardController *KeyboardController =
      static_cast<X86_GenericKeyboardController *>(device);
  switch (port) {
  case 0x64:
    return KeyboardController->status;
  default:
    printf("ERROR: Bad KeyboardController port %04Xh!\n", port);
    exit(-1);
  }
}

void X86_GenericKeyboardController::Write8Impl(void *device, uint16_t port,
                                               uint8_t val) {
  X86_GenericKeyboardController *KeyboardController =
      static_cast<X86_GenericKeyboardController *>(device);
  switch (port) {
  default:
    break;
  }
  printf("ERROR: Bad KeyboardController port %04Xh!\n", port);
  exit(-1);
}

uint16_t X86_GenericKeyboardController::Read16Impl(void *device,
                                                   uint16_t port) {
  X86_GenericKeyboardController *KeyboardController =
      static_cast<X86_GenericKeyboardController *>(device);
  printf("ERROR: Bad KeyboardController port %04Xh!\n", port);
  exit(-1);
}

void X86_GenericKeyboardController::Write16Impl(void *device, uint16_t port,
                                                uint16_t val) {
  X86_GenericKeyboardController *KeyboardController =
      static_cast<X86_GenericKeyboardController *>(device);
  printf("ERROR: Bad KeyboardController port %04Xh!\n", port);
  exit(-1);
}
