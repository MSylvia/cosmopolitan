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

/	Returns arc cosine of 𝑥.
/
/	@param	𝑥 is an 80-bit long double passed on stack in 16-bytes
/	@return	result of computation on FPU stack in %st
/	@define	atan2(abs(sqrt((1-𝑥)*(1+𝑥))),𝑥)
/	@domain	-1 ≤ 𝑥 ≤ 1
/	@mode	long,legacy
tinymath_acosl:
	push	%rbp
	mov	%rsp,%rbp
	.profilable
	fldl    16(%rbp)
	fld     %st
#ifdef	__FAST_MATH__
	fmul	%st(1),%st
	fsubrs	.Lone(%rip)
	fsqrt
#else
	fld1
	fsubp
	fld1
	fadd    %st(2)
	fmulp
	fsqrt
	fabs				# needed in downward rounding mode
#endif
	fxch
	fpatan
	pop	%rbp
	ret
	.endfn	tinymath_acosl,globl
	.alias	tinymath_acosl,acosl

	.rodata.cst4
.Lone:	.float	1.0
