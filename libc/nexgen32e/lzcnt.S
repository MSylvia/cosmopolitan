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

/	Finds leading bits in 𝑥.
/
/	@param	edi is 32-bit unsigned 𝑥 value
/	@return	eax number in range [0,32) or 32 if 𝑥 is 0
/	@see	also treasure trove of nearly identical functions
/
/	      uint32 𝑥   bsf(𝑥) tzcnt(𝑥)   ffs(𝑥)   bsr(𝑥) lzcnt(𝑥)
/	    0x00000000      wut       32        0      wut       32
/	    0x00000001        0        0        1        0       31
/	    0x80000001        0        0        1       31        0
/	    0x80000000       31       31       32       31        0
/	    0x00000010        4        4        5        4       27
/	    0x08000010        4        4        5       27        4
/	    0x08000000       27       27       28       27        4
/	    0xffffffff        0        0        1       31        0
/
lzcnt:	.leafprologue
	.profilable
	mov	$31,%eax
	mov	$-1,%edx
	bsr	%edi,%ecx
	cmovz	%edx,%ecx
	sub	%ecx,%eax
	.leafepilogue
	.endfn	lzcnt,globl
	.source	__FILE__
