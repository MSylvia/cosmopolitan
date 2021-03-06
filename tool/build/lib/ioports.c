/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2020 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ This program is free software; you can redistribute it and/or modify         │
│ it under the terms of the GNU General Public License as published by         │
│ the Free Software Foundation; version 2 of the License.                      │
│                                                                              │
│ This program is distributed in the hope that it will be useful, but          │
│ WITHOUT ANY WARRANTY; without even the implied warranty of                   │
│ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU             │
│ General Public License for more details.                                     │
│                                                                              │
│ You should have received a copy of the GNU General Public License            │
│ along with this program; if not, write to the Free Software                  │
│ Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA                │
│ 02110-1301 USA                                                               │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/calls/struct/iovec.h"
#include "libc/nexgen32e/uart.internal.h"
#include "libc/sysv/consts/fileno.h"
#include "tool/build/lib/ioports.h"

static int OpE9Read(struct Machine *m) {
  int fd;
  uint8_t b;
  fd = STDIN_FILENO;
  if (fd >= m->fds.i) return -1;
  if (!m->fds.p[fd].cb) return -1;
  if (m->fds.p[fd].cb->readv(m->fds.p[fd].fd, &(struct iovec){&b, 1}, 1) == 1) {
    return b;
  } else {
    return -1;
  }
}

static void OpE9Write(struct Machine *m, uint8_t b) {
  int fd;
  fd = STDOUT_FILENO;
  if (fd >= m->fds.i) return;
  if (!m->fds.p[fd].cb) return;
  m->fds.p[fd].cb->writev(m->fds.p[fd].fd, &(struct iovec){&b, 1}, 1);
}

static int OpSerialIn(struct Machine *m, int r) {
  switch (r) {
    case UART_DLL:
      if (!m->dlab) {
        return OpE9Read(m);
      } else {
        return 0x01;
      }
    case UART_LSR:
      return UART_TTYDA | UART_TTYTXR | UART_TTYIDL;
    default:
      return 0;
  }
}

static void OpSerialOut(struct Machine *m, int r, uint32_t x) {
  switch (r) {
    case UART_DLL:
      if (!m->dlab) {
        return OpE9Write(m, x);
      }
      break;
    case UART_LCR:
      m->dlab = !!(x & UART_DLAB);
      break;
    default:
      break;
  }
}

uint64_t OpIn(struct Machine *m, uint16_t p) {
  switch (p) {
    case 0xE9:
      return OpE9Read(m);
    case 0x3F8 ... 0x3FF:
      return OpSerialIn(m, p - 0x3F8);
    default:
      return -1;
  }
}

void OpOut(struct Machine *m, uint16_t p, uint32_t x) {
  switch (p) {
    case 0xE9:
      OpE9Write(m, x);
      break;
    case 0x3F8 ... 0x3FF:
      OpSerialOut(m, p - 0x3F8, x);
      break;
    default:
      break;
  }
}
