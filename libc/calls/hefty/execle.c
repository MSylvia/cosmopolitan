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
#include "libc/calls/hefty/mkvarargv.h"
#include "libc/mem/mem.h"

/**
 * Executes program, with custom environment.
 *
 * The current process is replaced with the executed one.
 *
 * @param prog will not be PATH searched, see commandv()
 * @param arg[0] is the name of the program to run
 * @param arg[1,n-3] optionally specify program arguments
 * @param arg[n-2] is NULL
 * @param arg[n-1] is a pointer to a ["key=val",...,NULL] array
 * @return doesn't return on success, otherwise -1 w/ errno
 * @notasyncsignalsafe (TODO)
 */
int execle(const char *exe, const char *arg,
           ... /*, NULL, char *const envp[] */) {
  va_list va;
  void *argv;
  va_start(va, arg);
  if ((argv = mkvarargv(arg, va))) {
    execve(exe, argv, va_arg(va, char *const *));
    free(argv);
  }
  va_end(va);
  return -1;
}
