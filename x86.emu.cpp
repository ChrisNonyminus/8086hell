#include "x86.h"
#include "devices/kbc.h"
#include "x86.io.h"
#include <assert.h>
#include <cstdlib>

#include "devices/cmos.h"

X86_EMU_Machine *X86_EMU_gActiveMachine;

static void Deinit_X86_Common() {
  if (!X86_EMU_gActiveMachine)
    return;
  X86_EMU_gActiveMachine->io_devices.clear();
}

static int X86_IO_HandlerCmp(const void *lhs, const void *rhs) {

  X86_IODeviceBase const *const l =
      static_cast<X86_IODeviceBase const *const>(lhs);
  X86_IODeviceBase const *const r =
      static_cast<X86_IODeviceBase const *const>(rhs);

  if ((l->port_end < r->port_start) && (l->port_start < r->port_end))
    return -1;
  else if ((l->port_end > r->port_start) && (l->port_start > r->port_end))
    return 1;
  else {
    return 0;
  }
}

static uint8_t X86_IO_Read8(uint16_t port) {
  X86_IODeviceBase key = {port, static_cast<uint16_t>(port + 1)};
  const X86_IODeviceBase *res = static_cast<const X86_IODeviceBase *>(
      bsearch(&key, X86_EMU_gActiveMachine->io_devices.data(),
              X86_EMU_gActiveMachine->io_devices.size(),
              sizeof(X86_IODeviceBase), X86_IO_HandlerCmp));
  if (res) {
    return res->Read8(const_cast<X86_IODeviceBase *>(res), port);
  }

  printf("X86_IO_Read8: Unknown IO port %04Xh!\n", port);
  return -1;
}

static void X86_IO_Write8(uint16_t port, uint8_t val) {
  X86_IODeviceBase key = {port, static_cast<uint16_t>(port + 1)};
  const X86_IODeviceBase *res = static_cast<const X86_IODeviceBase *>(
      bsearch(&key, X86_EMU_gActiveMachine->io_devices.data(),
              X86_EMU_gActiveMachine->io_devices.size(),
              sizeof(X86_IODeviceBase), X86_IO_HandlerCmp));
  if (res) {
    res->Write8(const_cast<X86_IODeviceBase *>(res), port, val);
    return;
  }

  printf("X86_IO_Read8: Unknown IO port %04Xh!\n", port);
}

static uint16_t X86_IO_Read16(uint16_t port) {
  assert((port % 2) == 0);
  X86_IODeviceBase key = {port, static_cast<uint16_t>(port + 2)};
  const X86_IODeviceBase *res = static_cast<const X86_IODeviceBase *>(
      bsearch(&key, X86_EMU_gActiveMachine->io_devices.data(),
              X86_EMU_gActiveMachine->io_devices.size(),
              sizeof(X86_IODeviceBase), X86_IO_HandlerCmp));
  if (res) {
    return res->Read16(const_cast<X86_IODeviceBase *>(res), port);
  }

  printf("X86_IO_Read8: Unknown IO port %04Xh!\n", port);
  return -1;
}

static void X86_IO_Write16(uint16_t port, uint16_t val) {
  assert((port % 2) == 0);
  X86_IODeviceBase key = {port, static_cast<uint16_t>(port + 2)};
  const X86_IODeviceBase *res = static_cast<const X86_IODeviceBase *>(
      bsearch(&key, X86_EMU_gActiveMachine->io_devices.data(),
              X86_EMU_gActiveMachine->io_devices.size(),
              sizeof(X86_IODeviceBase), X86_IO_HandlerCmp));
  if (res) {
    res->Write16(const_cast<X86_IODeviceBase *>(res), port, val);
    return;
  }

  printf("X86_IO_Read8: Unknown IO port %04Xh!\n", port);
}

static X86_EMU_Machine *Init_80286() {
  X86_EMU_Machine *machine = new X86_EMU_Machine;

  //   static X86_IODeviceBase handlers[] = {
  //       {0xF100, 0xF102, "F100", 0, NULL, X86_IO_F100_Read8,
  //       X86_IO_F100_Write8,
  //        X86_IO_F100_Read16, X86_IO_F100_Write16},
  //   };

  //   qsort(handlers, sizeof(handlers) / sizeof(handlers[0]),
  //         sizeof(X86_IODeviceBase), X86_IO_HandlerCmp);
  //   for (int i = 0; i < sizeof(handlers) / sizeof(handlers[0]); i++) {
  //     handlers[i].parent = machine;
  //   }

  //   machine->io_handlers = handlers;
  //   machine->num_io_handlers = sizeof(handlers) / sizeof(handlers[0]);
  static X86_IODeviceBase testF100 = {0xF100,
                                      0xF102,
                                      "F100",
                                      0,
                                      NULL,
                                      X86_IO_F100_Read8,
                                      X86_IO_F100_Write8,
                                      X86_IO_F100_Read16,
                                      X86_IO_F100_Write16};

  machine->io_devices.push_back(testF100);
  machine->io_devices.push_back({0x80, 0x81, "Manufacturing Diagnostics port",
                                 0, NULL, X86_IO_80_Read8, X86_IO_80_Write8,
                                 X86_IO_80_Read16, X86_IO_80_Write16});
  machine->io_devices.push_back(X86_GenericCMOS());
  machine->io_devices.push_back(X86_GenericKeyboardController());

  qsort(machine->io_devices.data(), machine->io_devices.size(),
        sizeof(X86_IODeviceBase), X86_IO_HandlerCmp);
  for (auto &dev : machine->io_devices) {
    dev.parent = machine;
  }

  machine->Deinit = Deinit_X86_Common;
  machine->IO_Read8 = X86_IO_Read8;
  machine->IO_Write8 = X86_IO_Write8;
  machine->IO_Read16 = X86_IO_Read16;
  machine->IO_Write16 = X86_IO_Write16;
  return machine;
}

BOOL X86_EMU_InitMachine(X86_EMU_MachineType type) {
  switch (type) {
  case X86_MACHINE_80286_PCAT:
    X86_EMU_gActiveMachine = Init_80286();
    break;
  default:
    printf("X86_EMU_InitMachine: Unknown X86 machine type! this shouldn't "
           "happen.\n");
    return FALSE;
  }
  return !!X86_EMU_gActiveMachine;
}

BOOL X86_EMU_FreeMachine(void) {
  X86_EMU_gActiveMachine->Deinit();
  delete X86_EMU_gActiveMachine;
  return !X86_EMU_gActiveMachine;
}
