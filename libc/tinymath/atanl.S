/*-*- mode:unix-assembly; indent-tabs-mode:t; tab-width:8; coding:utf-8     -*-│
│vi: set et ft=asm ts=8 sw=8 fenc=utf-8                                     :vi│
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
#include "libc/macros.h"
.source	__FILE__

/	Returns arc tangent of 𝑥.
/
/	@param	𝑥 is an 80-bit long double passed on stack in 16-bytes
/	@return	result of computation on FPU stack in %st
/	@define	atan(𝑥) = Σₙ₌₀₋∞ 2²ⁿ(𝑛!)²/(𝟸𝑛+𝟷)!(𝑥²ⁿ⁺¹/(𝑥²+𝟷)ⁿ⁺¹)
/		              1  3   1  5   1  7   1  9    1  11
/	@define	atan(𝑥) = 𝑥 - - 𝑥  + - 𝑥  - - 𝑥  + - 𝑥  - -- 𝑥   ...
/		              3      5      7      9      11
tinymath_atanl:
	push	%rbp
	mov	%rsp,%rbp
	.profilable
	fldt	16(%rbp)
	fld1
	fpatan
	pop	%rbp
	ret
	.endfn	tinymath_atanl,globl
	.alias	tinymath_atanl,atanl
