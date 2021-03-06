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
#include "libc/bits/bits.h"
#include "libc/calls/calls.h"
#include "libc/calls/struct/stat.h"
#include "libc/calls/struct/timeval.h"
#include "libc/conv/conv.h"
#include "libc/dce.h"
#include "libc/errno.h"
#include "libc/fmt/fmt.h"
#include "libc/log/internal.h"
#include "libc/log/log.h"
#include "libc/math.h"
#include "libc/nexgen32e/nexgen32e.h"
#include "libc/runtime/runtime.h"
#include "libc/stdio/stdio.h"
#include "libc/str/str.h"
#include "libc/sysv/consts/fileno.h"
#include "libc/time/struct/tm.h"
#include "libc/time/time.h"

#define kNontrivialSize (8 * 1000 * 1000)

STATIC_YOINK("ntoa");
STATIC_YOINK("stoa");
STATIC_YOINK("ftoa");

static int loglevel2char(unsigned level) {
  switch (level) {
    case kLogInfo:
      return 'I';
    case kLogDebug:
      return 'D';
    case kLogWarn:
      return 'W';
    case kLogFatal:
      return 'F';
    case kLogVerbose:
      return 'V';
    default:
      return '?';
  }
}

/**
 * Takes corrective action if logging is on the fritz.
 */
void vflogf_onfail(FILE *f) {
  errno_t err;
  struct stat st;
  if (IsTiny()) return;
  err = ferror(f);
  if ((err == ENOSPC || err == EDQUOT || err == EFBIG) &&
      (fstat(fileno(f), &st) == -1 || st.st_size > kNontrivialSize)) {
    ftruncate(fileno(f), 0);
    fseek(f, SEEK_SET, 0);
    f->beg = f->end = 0;
    clearerr(f);
    (fprintf)(f, "performed emergency log truncation: %s\r\n", strerror(err));
  }
}

/**
 * Writes formatted message w/ timestamp to log.
 */
void(vflogf)(unsigned level, const char *file, int line, FILE *f,
             const char *fmt, va_list va) {
  static struct timespec ts;
  struct tm tm;
  long double t2;
  const char *prog;
  int64_t secs, nsec, dots;
  char timebuf[32], *timebufp;
  if (!f) f = g_logfile;
  if (fileno(f) == -1) return;
  t2 = nowl();
  secs = t2;
  nsec = rem1000000000int64(t2 * 1e9L);
  if (secs > ts.tv_sec) {
    localtime_r(&secs, &tm);
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%dT%H:%M:%S.", &tm);
    timebufp = timebuf;
    dots = nsec;
  } else {
    timebufp = "--------------------";
    dots = nsec - ts.tv_nsec;
  }
  ts.tv_sec = secs;
  ts.tv_nsec = nsec;
  prog = basename(program_invocation_name);
  if ((fprintf)(f, "%c%s%06ld:%s:%d:%.*s:%d] ", loglevel2char(level), timebufp,
                rem1000000int64(div1000int64(dots)), file, line,
                strchrnul(prog, '.') - prog, prog, getpid()) <= 0) {
    vflogf_onfail(f);
  }
  (vfprintf)(f, fmt, va);
  va_end(va);
  fputs("\r\n", f);
  if (level == kLogFatal) {
    __start_fatal(file, line);
    (dprintf)(STDERR_FILENO, "fatal error see logfile\r\n");
    __die();
    unreachable;
  }
}
