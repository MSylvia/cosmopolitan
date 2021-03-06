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
#include "libc/calls/calls.h"
#include "libc/calls/hefty/internal.h"
#include "libc/calls/internal.h"
#include "libc/dce.h"
#include "libc/sysv/consts/at.h"
#include "libc/sysv/errfuns.h"

/**
 * Checks if effective user can access path in particular ways.
 *
 * @param dirfd is usually AT_FDCWD
 * @param path is a filename or directory
 * @param flags can be R_OK, W_OK, X_OK, F_OK
 * @return 0 if ok, or -1 and sets errno
 * @asyncsignalsafe
 */
int faccessat(int dirfd, const char *path, int mode, uint32_t flags) {
  if (!path) return efault();
  if (!IsWindows()) {
    return faccessat$sysv(dirfd, path, mode, flags);
  } else {
    return faccessat$nt(dirfd, path, mode, flags);
  }
}
