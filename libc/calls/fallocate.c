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
#include "libc/calls/internal.h"
#include "libc/dce.h"
#include "libc/nt/enum/fsctl.h"
#include "libc/nt/files.h"
#include "libc/nt/struct/filezerodatainformation.h"
#include "libc/sysv/consts/falloc.h"
#include "libc/sysv/errfuns.h"

/**
 * Manipulates underlying physical medium of file.
 *
 * This system call generalizes to many powerful use cases on Linux,
 * such as creating gigantic sparse files that take up little space.
 * This API can polyfill a certain subset of parameters safely, e.g.
 * ones identical to ftruncate(), but errs on the side of caution.
 *
 * @param fd must be open for writing
 * @param mode can be 0, FALLOC_xxx
 * @param length is how much physical space to reserve / commit
 * @return 0 on success, or -1 w/ errno
 * @see ftruncate()
 */
int fallocate(int fd, int32_t mode, int64_t offset, int64_t length) {
  int rc;
  uint32_t br;
  if (mode == -1 /* our sysvconsts definition */) return eopnotsupp();
  if (!mode && !length) return ftruncate(fd, offset);
  if (IsLinux()) {
    rc = fallocate$sysv(fd, mode, offset, length);
    if (rc == 0x011d) rc = enosys(); /*RHEL5:CVE-2010-3301*/
    return rc;
  } else if (!IsWindows()) {
    return posix_fallocate$sysv(fd, offset, length);
  } else if (IsWindows()) {
    if (!__isfdkind(fd, kFdFile)) return ebadf();
    if (mode == FALLOC_FL_ZERO_RANGE) {
      if (DeviceIoControl(
              g_fds.p[fd].handle, kNtFsctlSetZeroData,
              &(struct NtFileZeroDataInformation){offset, offset + length},
              sizeof(struct NtFileZeroDataInformation), NULL, 0, &br, NULL)) {
        return 0;
      } else {
        return __winerr();
      }
    } else if (!mode && !offset) {
      /*
       * this should commit physical space
       * but not guaranteed zero'd like linux
       */
      return ftruncate$nt(fd, length);
    } else {
      return eopnotsupp();
    }
  } else {
    return enosys();
  }
}
