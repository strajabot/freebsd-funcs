/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2024 Strahinja Stanisic <strajabot@FreeBSD.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
 * uses haszero(v) (((v) - 0x01010101UL) & ~(v) & 0x80808080UL)
 * which evalutates > 0 when there is zero in v
 * 
 * register a0 - char* str
 */
.globl strlen_riscv
.type strlen_riscv, @function
strlen_riscv:
	
	/* 
	 * register a0 - char*	str_start
	 * register a1 - char*	str_ptr
	 * register a2 - char[8]	iter
	 */


	/* load constants for haszero */
	li t0, 0x0101010101010101
	slli t1, t0, 7				# 0x8080808080808080, avoid li

	/* check alignment of str_start */
	andi a1, a0, ~0b111
	ld a2, (a1)
	beq a1, a0, .Lhas_zero

	/* fill bytes before str_start with non-zero */
	slli t2, a0, 3
	addi t3, t2, -64
	neg t3, t3
	srl t3, t0, t3		
	or a2, a2, t3

	/* unrolled iteration of haszero */
	not t2, a2
	sub a2, a2, t0
	and a2, a2, t2
	and a2, a2, t1

	bnez a2, .Lfind_zero

.Lloop_has_zero:
	ld a2, 8(a1)
	addi a1, a1, 8	# move ptr to next 8byte
.Lhas_zero:
	not t2, a2
	sub a2, a2, t0
	and a2, a2, t2
	and a2, a2, t1

	beqz a2, .Lloop_has_zero

.Lfind_zero:
	/* use (iter & -iter) to isolate lowest set bit */
	sub a3, zero, a2	#a3 = -iter
	and t1, a2, a3		#t1 = (iter & -iter)

	li t0, 0x0001020304050607
	srli t1, t1, 7
	/* 
	 * lowest set bit is 2^(8*k)
	 * multiplying by it shifts the idx array in t0 by k bytes to the left
	 */
	mul	t1, t1, t0
	/* highest byte contains idx of first zero */
	srli t1, t1, 56

	add a1, a1, t1
	sub a0, a1, a0
	ret

