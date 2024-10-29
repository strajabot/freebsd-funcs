
/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2024 Strahinja Stanisic <strajabot@FreeBSD.org>
 */

/*
 * a0 - const void* b
 * a1 - int c
 * a2 - size_t len
 */
.globl memrchr_riscv
.type memrchr_riscv, @function
memrchr_riscv:
	/*
	 * a0 - const void* b
	 * a1 - char cccccccc[8]
	 * a2 - const uint64_t* ptr
	 * a3 -	char iter[8]
	 * a4 - void* start_align 
	 */

	/* if(len == 0) return NULL; */ 
	beqz a2, .Lno_match

	/* int to char */
	andi a1, a1, 0xFF

	/* t0 = 0x0101010101010101 */
	/* t1 = 0x8080808080808080 */
	li t0, 0x01010101
	slli t1, t0, 32
	or t0, t0, t1
	slli t1, t0, 7
	
	/* spread char accross bytes */
	mul a1, a1, t0
	
	/* start_align = b & ~0b111 */
	andi a4, a0, ~0b111

	/* end = b + len 
	 * ptr = (end + 7) & ~0b111 
	 * mask_shift = ((-end) << 3) & 0b111111
	 * mask = REP8_0x01 ^ (REP8_0x01 >> mask_shift)
	 * iter = iter & cccccccc
	 * iter = iter | mask
	 * ptr = ptr - 8
	 */
	add t2, a0, a2
	add a2, t2, 7
	neg t2, t2
	andi a2, a2, ~0b111
	slli t2, t2, 3
	ld a3, -8(a2)
	srl t3, t0, t2
	xor a3, a3, a1
	xor t2, t0, t3
	addi a2, a2, -8
	or a3, a3, t2

	not t2, a3
	sub t3, a3, t0
	and t2, t2, t1
	and t3, t3, t2

	bnez t3, .Lfind_zero

	beq a2, a4, .Lno_match

	sub t2, a2, a4
	ld a3, -8(a2)
	and t2, t2, 0b1111
	xor a3, a3, a1
	addi a2, a2, -8

	not t3, a3
	sub t4, a3, t0
	and t3, t3, t1
	sub t2, a2, t2
	and t4, t4, t3

	bnez t4, .Lfind_zero

	add a2, t2, 8

	beq a2, a4, .Lno_match
.Lloop:
	ld a3, -8(a2)
	ld a5, -16(a2)

	addi a2, a2, -8

	xor a3, a3, a1
	xor a5, a5, a1

	not t2, a3
	not a6, a5
	
	sub t3, a3, t0
	sub a7, a5, t0
	
	and t2, t2, t1
	and a6, a6, t1
	
	and t3, t3, t2
	and a7, a7, a6

	bnez t3, .Lfind_zero

	addi a2, a2, -8
	mv a3, a5

	bnez a7, .Lfind_zero

	bne a2, a4, .Lloop

.Lno_match:
	li a0, 0
	ret
.Lfind_zero:
	addi a2, a2, 7	
.Llfind_zero:
	srli a1, a3, 56
	beqz a1, .Lfound_match
	slli a3, a3, 8
	addi a2, a2, -1
	bnez a3, .Llfind_zero
.Lfound_match:
	sltu a0, a2, a0
	addi a0, a0, -1
	and a0, a0, a2
	ret

