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
#include "libc/calls/internal.h"
#include "libc/dce.h"
#include "libc/sysv/consts/at.h"
#include "libc/sysv/errfuns.h"

/**
 * Changes permissions on file, e.g.:
 *
 *   CHECK_NE(-1, chmod("foo/bar.txt", 0644));
 *   CHECK_NE(-1, chmod("o/default/program.com", 0755));
 *   CHECK_NE(-1, chmod("privatefolder/", 0700));
 *
 * The esoteric bits generally available on System Five are:
 *
 *   CHECK_NE(-1, chmod("/opt/", 01000));          // sticky bit
 *   CHECK_NE(-1, chmod("/usr/bin/sudo", 04755));  // setuid bit
 *   CHECK_NE(-1, chmod("/usr/bin/wall", 02755));  // setgid bit
 *
 * This works on Windows NT if you ignore the error ;-)
 *
 * @param pathname must exist
 * @param mode contains octal flags (base 8)
 * @errors ENOENT, ENOTDIR, ENOSYS
 * @asyncsignalsafe
 * @see fchmod()
 */
int chmod(const char *pathname, uint32_t mode) {
  if (!pathname) return efault();
  return fchmodat$sysv(AT_FDCWD, pathname, mode, 0);
}
