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
#include "libc/bits/progn.internal.h"
#include "libc/bits/safemacros.internal.h"
#include "libc/str/str.h"
#include "libc/testlib/testlib.h"
#include "libc/x/x.h"

/**
 * Turns string into code.
 */
nodiscard testonly char *testlib_formatstr(size_t cw, const void *s, int n) {
  switch (cw) {
    case 1:
      if (n == -1) n = s ? strlen(s) : 0;
      return xasprintf("%`'.*s", n, s);
    case 2:
      if (n == -1) n = s ? strlen16(s) : 0;
      return xasprintf("%`'.*hs", n, s);
    case 4:
      if (n == -1) n = s ? wcslen(s) : 0;
      return xasprintf("%`'.*ls", n, s);
    default:
      abort();
  }
}
