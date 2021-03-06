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
#include "libc/assert.h"
#include "libc/bits/safemacros.internal.h"
#include "libc/calls/calls.h"
#include "libc/calls/hefty/spawn.h"
#include "libc/calls/struct/iovec.h"
#include "libc/macros.h"
#include "libc/runtime/runtime.h"
#include "libc/sock/sock.h"
#include "libc/str/str.h"
#include "tool/build/lib/demangle.h"

struct CxxFilt {
  int pid;
  int fds[3];
} g_cxxfilt;

void CloseCxxFilt(void) {
  close(g_cxxfilt.fds[0]);
  close(g_cxxfilt.fds[1]);
  g_cxxfilt.pid = -1;
}

void SpawnCxxFilt(void) {
  int pid;
  const char *cxxfilt;
  char path[PATH_MAX];
  cxxfilt = firstnonnull(getenv("CXXFILT"), "c++filt");
  if (commandv(cxxfilt, path)) {
    g_cxxfilt.fds[0] = -1;
    g_cxxfilt.fds[1] = -1;
    g_cxxfilt.fds[2] = 2;
    if ((pid = spawnve(0, g_cxxfilt.fds, path, (char *const[]){cxxfilt, NULL},
                       environ)) != -1) {
      atexit(CloseCxxFilt);
    }
  } else {
    pid = -1;
  }
  g_cxxfilt.pid = pid;
}

char *CopySymbol(char *p, size_t pn, const char *s, size_t sn) {
  size_t extra;
  bool showdots, iscomplicated;
  assert(pn >= 1 + 3 + 1 + 1);
  iscomplicated = memchr(s, ' ', sn) || memchr(s, '(', sn);
  extra = 1;
  if (iscomplicated) extra += 2;
  if (sn + extra > pn) {
    sn = pn - extra - 3;
    showdots = true;
  } else {
    showdots = false;
  }
  if (iscomplicated) *p++ = '"';
  p = mempcpy(p, s, sn);
  if (showdots) p = stpcpy(p, "...");
  if (iscomplicated) *p++ = '"';
  *p = '\0';
  return p;
}

char *DemangleCxxFilt(char *p, size_t pn, const char *s, size_t sn) {
  ssize_t rc;
  size_t got;
  struct iovec iov[2];
  static char buf[PAGESIZE];
  if (!g_cxxfilt.pid) SpawnCxxFilt();
  if (g_cxxfilt.pid == -1) return NULL;
  buf[0] = '\n';
  iov[0].iov_base = s;
  iov[0].iov_len = sn;
  iov[1].iov_base = buf;
  iov[1].iov_len = 1;
  writev(g_cxxfilt.fds[0], iov, ARRAYLEN(iov));
  if ((rc = read(g_cxxfilt.fds[1], buf, sizeof(buf))) != -1) {
    got = rc;
    if (got >= 2 && buf[got - 1] == '\n') {
      if (buf[got - 2] == '\r') --got;
      --got;
      return CopySymbol(p, pn, buf, got);
    }
  }
  CloseCxxFilt();
  return NULL;
}

/**
 * Decrypts C++ symbol.
 *
 * Decoding these takes roughly the same lines of code as an entire
 * x86_64 disassembler. That's just for the GNU encoding scheme. So
 * what we'll do, is just offload this work to the c++filt program.
 */
char *Demangle(char *p, const char *symbol, size_t n) {
  char *r;
  size_t sn;
  sn = strlen(symbol);
  if (startswith(symbol, "_Z")) {
    if ((r = DemangleCxxFilt(p, n, symbol, sn))) return r;
  }
  return CopySymbol(p, n, symbol, sn);
}
