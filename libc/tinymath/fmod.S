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

/	fmod [sic] does (𝑥 rem 𝑦) w/ round()-style rounding.
/
/	@param	𝑥 is double passed in lower half of %xmm0
/	@param	𝑦 is double passed in lower half of %xmm1
/	@return	remainder ∈ (-|𝑦|,|𝑦|) in %xmm0
/	@define	𝑥-trunc(𝑥/𝑦)*𝑦
/	@see	emod()
tinymath_fmod:
	ezlea	tinymath_fmodl,ax
	jmp	_d2ld2
	.endfn	tinymath_fmod,globl
	.alias	tinymath_fmod,fmod
