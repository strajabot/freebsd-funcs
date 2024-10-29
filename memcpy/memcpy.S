/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2024 Strahinja Stanisic <strajabot@FreeBSD.org>
 */


/* exectued when dst - src is multiple of 8
 * a0 - void* dst
 * a1 - void* src
 * a2 - size_t len
 */
.Lmemcpy8:
	mv a3, a0

	/* loop until dst becomes aligned, or len becomes 0, whichever is first */
	
	/* t0 = MIN((-dst) & 0b111, len); */
	neg t0, a0
	andi t0, t0, 0b111
	sub t0, t0, a2
	srai t1, t0, 63
	and t0, t0, t1
	add t0, a2, t0

	sub a2, a2, t0

	beqz t0, .Llstart8_skip 
.Llstart8:
	lb t1, (a1)
	addi t0, t0, -1
	addi a1, a1, 1
	sb t1, (a3)
	addi a3, a3, 1
	bnez t0, .Llstart8

.Llstart8_skip:
	beqz a2, .Lreturn8
	
	/* a4 - uint64_t* end_align */
	
	/* end_align = (dst + len) & ~0b111 */
	add a4, a3, a2
	andi a4, a4, ~0b111

	/* len = len % 8 */
	andi a2, a2, 0b111
	
	beq a3, a4, .Llmain8_skip
.Llmain8:
	ld t0, (a1)
	sd t0, (a3)
	addi a3, a3, 8
	addi a1, a1, 8
	bne a3, a4, .Llmain8

.Llmain8_skip:
	
	beqz a2, .Llend8_skip 
.Llend8:
	lb t1, (a1)
	addi a2, a2, -1
	addi a1, a1, 1
	sb t1, (a3)
	addi a3, a3, 1
	bnez a2, .Llend8

.Llend8_skip:
	
.Lreturn8:
	ret

.Lmemcpy4:
	mv a3, a0

	/* loop until dst becomes aligned, or len becomes 0, whichever is first */
	
	/* t0 = MIN((-dst) & 0b11, len); */
	neg t0, a0
	andi t0, t0, 0b11
	sub t0, t0, a2
	srai t1, t0, 63
	and t0, t0, t1
	add t0, a2, t0

	sub a2, a2, t0

	beqz t0, .Llstart4_skip 
.Llstart4:
	lb t1, (a1)
	addi t0, t0, -1
	addi a1, a1, 1
	sb t1, (a3)
	addi a3, a3, 1
	bnez t0, .Llstart4

.Llstart4_skip:
	beqz a2, .Lreturn4
	
	/* a4 - uint64_t* end_align */
	
	/* end_align = (dst + len) & ~0b111 */
	add a4, a3, a2
	andi a4, a4, ~0b11

	/* len = len % 4 */
	andi a2, a2, 0b11
	
	beq a3, a4, .Llmain4_skip
.Llmain4:
	lw t0, (a1)
	sw t0, (a3)
	addi a3, a3, 4
	addi a1, a1, 4
	bne a3, a4, .Llmain4

.Llmain4_skip:
	
	beqz a2, .Llend4_skip 
.Llend4:
	lb t1, (a1)
	addi a2, a2, -1
	addi a1, a1, 1
	sb t1, (a3)
	addi a3, a3, 1
	bnez a2, .Llend4

.Llend4_skip:
	
.Lreturn4:
	ret

/*
 * a0 - void* dst
 * a1 - void* src
 * a2 - size_t len
 */
.globl memcpy_riscv
.type memcpy_riscv, @function
memcpy_riscv:
	beqz a2, .Lreturn

	/* diff = (dstv - srcv) & 0b111 */
	sub t0, a0, a1
	andi t0, t0, 0b111

	beqz t0, .Lmemcpy8

	addi t0, t0, -4
	beqz t0, .Lmemcpy4

	/* we never change a0, because memcpy returns the original dst */
	mv a3, a0

	/* loop until dst becomes aligned, or len becomes 0, whichever is first */
	
	/* t0 = MIN((-dst) & 0b111, len); */
	neg t0, a0
	andi t0, t0, 0b111
	sub t0, t0, a2
	srai t1, t0, 63
	and t0, t0, t1
	add t0, a2, t0

	sub a2, a2, t0

	beqz t0, .Llstart_skip 
.Llstart:
	lb t1, (a1)
	addi t0, t0, -1
	addi a1, a1, 1
	sb t1, (a3)
	addi a3, a3, 1
	bnez t0, .Llstart

.Llstart_skip:
	beqz a2, .Lreturn

	/* 
	 * a4 - size_t right_shift
	 * a5 - size_t left_shift
	 * a6 - size_t whole (number od dword stores)
	 */
	
	/* right_shift = (src % 0b111) * 8; */
	andi a4, a1, 0b111
	slli a4, a4, 3

	/* left_shift = 64 - right_shift */
	neg a5, a4
	
	/* whole = len / 8 */
	srli a6, a2, 3

	/* len = len % 8 */
	andi a2, a2, 0b111

	/* t0 - uint64_t* ptr */

	/* ptr = src & ~0b111 */
	andi t0, a1, ~0b111
	
	/* src += whole * 8 */
	slli t1, a6, 3 
	add a1, a1, t1

	/*
	 * t1 - uint64_t low
	 * t2 - uint64_t high
	 */

	/* low = *ptr++ */
	ld t1, (t0)
	addi t0, t0, 8

	/* low >>= right_shift */
	srl t1, t1, a4

	beqz a6, .Llmain_skip
.Llmain:
	/* high = *ptr++ */
	ld t2, (t0)
	addi t0, t0, 8
	
	/* whole-- */
	addi a6, a6, -1

	/* temp = (high << left_shift) | low */
	sll t3, t2, a5
	or t3, t3, t1
	
	/* low = high >> right_shift */
	srl t1, t2, a4

	/* *dst++ = temp */
	sd t3, (a3)
	addi a3, a3, 8

	bnez a6, .Llmain

.Llmain_skip:

	beqz a2, .Llend_skip 
.Llend:
	lb t1, (a1)
	addi a2, a2, -1
	addi a1, a1, 1
	sb t1, (a3)
	addi a3, a3, 1
	bnez a2, .Llend

.Llend_skip:

.Lreturn:
	ret


