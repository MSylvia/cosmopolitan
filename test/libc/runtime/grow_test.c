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
#include "libc/bits/pushpop.h"
#include "libc/limits.h"
#include "libc/macros.h"
#include "libc/mem/mem.h"
#include "libc/stdio/stdio.h"
#include "libc/str/str.h"
#include "libc/testlib/testlib.h"

TEST(grow, testNull_hasAllocatingBehavior) {
  void *p = NULL;
  size_t capacity = 0;
  EXPECT_TRUE(__grow(&p, &capacity, 1, 0));
  EXPECT_NE(NULL, p);
  EXPECT_EQ(32, capacity);
  free_s(&p);
}

TEST(grow, testCapacity_isInUnits_withTerminatorGuarantee) {
  void *p = NULL;
  size_t capacity = 0;
  EXPECT_TRUE(__grow(&p, &capacity, 8, 0));
  EXPECT_NE(NULL, p);
  EXPECT_EQ(32 / 8 + 1, capacity);
  free_s(&p);
}

TEST(grow, testStackMemory_convertsToDynamic) {
  int A[] = {1, 2, 3};
  int *p = A;
  size_t capacity = ARRAYLEN(A);
  if (!isheap(p)) {
    EXPECT_TRUE(__grow(&p, &capacity, sizeof(int), 0));
    EXPECT_TRUE(isheap(p));
    EXPECT_GT(capacity, ARRAYLEN(A));
    EXPECT_EQ(1, p[0]);
    EXPECT_EQ(2, p[1]);
    EXPECT_EQ(3, p[2]);
    p[0] = 7;
    EXPECT_EQ(1, A[0]);
    free(p);
  }
}

TEST(grow, testGrowth_clearsNewMemory) {
  size_t i, capacity = 123;
  char *p = malloc(capacity);
  memset(p, 'a', capacity);
  EXPECT_TRUE(__grow(&p, &capacity, 1, 0));
  EXPECT_GT(capacity, 123);
  for (i = 0; i < 123; ++i) ASSERT_EQ('a', p[i]);
  for (i = 123; i < capacity; ++i) ASSERT_EQ(0, p[i]);
  free_s(&p);
}

TEST(grow, testBonusParam_willGoAboveAndBeyond) {
  size_t capacity = 32;
  char *p = malloc(capacity);
  EXPECT_TRUE(__grow(&p, &capacity, 1, 0));
  EXPECT_LT(capacity, 1024);
  free_s(&p);
  p = malloc((capacity = 32));
  EXPECT_TRUE(__grow(&p, &capacity, 1, 1024));
  EXPECT_GT(capacity, 1024);
  free_s(&p);
}

TEST(grow, testOverflow_returnsFalseAndDoesNotFree) {
  int A[] = {1, 2, 3};
  int *p = A;
  size_t capacity = ARRAYLEN(A);
  if (!isheap(p)) {
    EXPECT_FALSE(__grow(&p, &capacity, pushpop(SIZE_MAX), 0));
    EXPECT_FALSE(isheap(p));
    EXPECT_EQ(capacity, ARRAYLEN(A));
    EXPECT_EQ(1, p[0]);
    EXPECT_EQ(2, p[1]);
    EXPECT_EQ(3, p[2]);
    free_s(&p);
  }
}
