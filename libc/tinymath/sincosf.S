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

/	Returns sine and cosine of 𝑥.
/
/	@param	𝑥 is float scalar in low quarter of %xmm0
/	@param	%rdi is float *out_sin
/	@param	%rsi is float *out_cos
/	@domain	-(3π/8) < 𝑥 < 3π/8 for best accuracy
tinymath_sincosf:
	push	%rbp
	mov	%rsp,%rbp
	.profilable
	push	%rax
	movss	%xmm0,4(%rsp)
	flds	4(%rsp)
	fsincos
	fxch
	fstps	(%rdi)
	fstps	(%rsi)
	leave
	ret
	.endfn	tinymath_sincosf,globl
	.alias	tinymath_sincosf,sincosf
