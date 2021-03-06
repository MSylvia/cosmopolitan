/*-*- mode:unix-assembly; indent-tabs-mode:t; tab-width:8; coding:utf-8     -*-│
│vi: set et ft=asm ts=8 tw=8 fenc=utf-8                                     :vi│
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

/	Returns 𝑥^𝑦.
/
/	@param	𝑥 is an 80-bit long double passed on stack in 16-bytes
/	@param	𝑦 is the power, also pushed on stack, in reverse order
/	@return	result of exponentiation on FPU stack in %st
/	@note	Sun's fdlibm needs 2kLOC to do this for RISC lool
tinymath_powl:
	push	%rbp
	mov	%rsp,%rbp
	.profilable
	fldt	32(%rbp)
	fldt	16(%rbp)
	fyl2x
	fld1
	fld	%st(1)
	fprem
	f2xm1
	faddp
	fscale
	fxch
	fstp	%st
	pop	%rbp
	ret
	.endfn	tinymath_powl,globl
	.alias	tinymath_powl,powl
	.alias	tinymath_powl,__powl_finite
