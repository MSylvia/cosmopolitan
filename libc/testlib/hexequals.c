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
#include "libc/str/str.h"
#include "libc/testlib/testlib.h"

/**
 * Tests that raw memory is equal to numeric representation, e.g.
 *
 *   testlib_hexequals("00010203", "\0\1\2\3", -1ul);
 *
 * @see unhexstr()
 */
testonly bool testlib_hexequals(const char *want, const void *got, size_t n) {
  size_t i;
  const unsigned char *p = (const unsigned char *)got;
  if (!got) return false;
  for (i = 0; i < n; ++i) {
    if (!want[i * 2]) break;
    if (i == n) break;
    if (p[i] != (unsigned char)(hextoint(want[i * 2 + 0]) * 16 +
                                hextoint(want[i * 2 + 1]))) {
      return false;
    }
  }
  return true;
}
