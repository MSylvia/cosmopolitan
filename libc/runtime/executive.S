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
#include "libc/dce.h"
#include "libc/macros.h"
#include "libc/notice.inc"
#include "libc/sysv/consts/prot.h"
#include "libc/sysv/consts/map.h"
#include "libc/dce.h"
.text.startup
.source	__FILE__

/	Stack frame that owns process from spawn to exit.
/
/	@param	edi is argc
/	@param	rsi is argv
/	@param	rdx is environ
/	@param	rcx is auxv
/	@noreturn
_executive:
	push	%rbp
	mov	%rsp,%rbp
	ezlea	_base,bx
	mov	%edi,%r12d
	mov	%rsi,%r13
	mov	%rdx,%r14
	mov	%rcx,%r15
	call	_spawn
	mov	%r12d,%edi
	mov	%r13,%rsi
	mov	%r14,%rdx
	mov	%r15,%rcx
	.weak	main
	call	main
	xchg	%eax,%edi
	call	exit
	.endfn	_executive,weak,hidden
	ud2

#ifdef __PG__
	.init.start 800,_init_ftrace
	push	%rdi
	push	%rsi
	mov	%r12d,%edi
	mov	%r13,%rsi
	call	ftrace_init
	mov	%eax,%r12d
	pop	%rsi
	pop	%rdi
	.init.end 800,_init_ftrace
#endif
