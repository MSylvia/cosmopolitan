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
#include "libc/str/str.h"
#include "libc/testlib/ezbench.h"
#include "libc/testlib/testlib.h"
#include "libc/unicode/unicode.h"

TEST(strwidth, testCjkWidesAndCombiningLowLines_withThompsonPikeEncoding) {
  /*───────────────────────────────────────────────────┬─*/
  EXPECT_EQ(20, strwidth(/**/ "𐌰𐌱𐌲𐌳𐌴𐌵𐌶𐌷▒▒▒▒▒▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(20, strwidth(/**/ "(╯°□°)╯𐄻︵ ̲┻̲━̲┻▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(20, strwidth(/**/ "ちゃぶ台返し▒▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(20, strclen(/*─*/ "𐌰𐌱𐌲𐌳𐌴𐌵𐌶𐌷▒▒▒▒▒▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(22, strclen(/*─*/ "(╯°□°)╯𐄻︵ ̲┻̲━̲┻▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(14, strclen(/*─*/ "ちゃぶ台返し▒▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(68, strlen(/*──*/ "𐌰𐌱𐌲𐌳𐌴𐌵𐌶𐌷▒▒▒▒▒▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(56, strlen(/*──*/ "(╯°□°)╯𐄻︵ ̲┻̲━̲┻▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(42, strlen(/*──*/ "ちゃぶ台返し▒▒▒▒▒▒▒▒" /*│*/));
  /*───────────────────────────────────────────────────┴─*/
}

TEST(strwidth16, testCjkWidesAndCombiningLowLines_lengthIsNotShorts) {
  /*──────────────────────────────────────────────────────┬─*/
  EXPECT_EQ(20, strwidth16(/**/ u"𐌰𐌱𐌲𐌳𐌴𐌵𐌶𐌷▒▒▒▒▒▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(20, strwidth16(/**/ u"(╯°□°)╯𐄻︵ ̲┻̲━̲┻▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(20, strwidth16(/**/ u"ちゃぶ台返し▒▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(20, strclen16(/*─*/ u"𐌰𐌱𐌲𐌳𐌴𐌵𐌶𐌷▒▒▒▒▒▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(22, strclen16(/*─*/ u"(╯°□°)╯𐄻︵ ̲┻̲━̲┻▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(14, strclen16(/*─*/ u"ちゃぶ台返し▒▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(28, strlen16(/*──*/ u"𐌰𐌱𐌲𐌳𐌴𐌵𐌶𐌷▒▒▒▒▒▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(23, strlen16(/*──*/ u"(╯°□°)╯𐄻︵ ̲┻̲━̲┻▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(14, strlen16(/*──*/ u"ちゃぶ台返し▒▒▒▒▒▒▒▒" /*│*/));
  /*──────────────────────────────────────────────────────┴─*/
}

TEST(wcwidth, testCjkWidesAndCombiningLowLines_widthIsNotLength) {
  /*────────────────────────────────────────────────────┬─*/
  EXPECT_EQ(20, wcswidth(/**/ L"Table flip▒▒▒▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(20, wcswidth(/**/ L"(╯°□°)╯︵ ̲┻̲━̲┻▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(20, wcswidth(/**/ L"ちゃぶ台返し▒▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(20, wcslen(/*──*/ L"Table flip▒▒▒▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(22, wcslen(/*──*/ L"(╯°□°)╯︵ ̲┻̲━̲┻▒▒▒▒▒▒▒" /*│*/));
  EXPECT_EQ(14, wcslen(/*──*/ L"ちゃぶ台返し▒▒▒▒▒▒▒▒" /*│*/));
  /*────────────────────────────────────────────────────┴─*/
}

TEST(wcwidth, block) {
  EXPECT_EQ(1, wcwidth(u'▄'));
}

TEST(strwidth, testTextDelimitingControlCodes_dontHaveSubstance) {
  EXPECT_EQ(0, strwidth("\0"));
  EXPECT_EQ(0, strwidth("\1"));
}

BENCH(wcwidth, bench) {
  volatile int x;
  EZBENCH2("wcwidth", donothing, x = wcwidth(VEIL("r", u'→')));
}
