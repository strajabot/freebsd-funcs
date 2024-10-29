/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2024 Strahinja Stanisic <strajabot@FreeBSD.org>
 */


/*
 * a0 - const void* b
 * a1 - int c;
 * a2 - size_t len;
 */
.globl memchr_riscv
.type memchr_riscv, @function
memchr_riscv:
	/*
     * a0 - const void* b;
     * a1 - char cccccccc[8];
	 * a2 -	size_t len; 
	 * a3 - size_t bound;
	 * a4 - char iter[8];
	 * a5 - size_t count;
	 * a6 - size_t remaining
	 */
	
	/* int to char */
	andi a1, a1, 0xFF
	
	/* strings where len < 8 handled here */
	sltiu t0, a2, 8
	beqz t0, .Lskip_lt_8  
.Llt_8:
	beqz a2, .Lno_match
	lb a4, (a0)
	addi a2, a2, -1
	addi a0, a0, 1
	bne a1, a4, .Llt_8
	addi a0, a0, -1
	ret
.Lskip_lt_8:
	
	/* t0 = 0x0101010101010101 */
	li t0, 0x01010101
	slli t1, t0, 32
	or t0, t0, t1

	/* t1 = 0x8080808080808080 */
	slli t1, t0, 7
		
	/* spread char accross bytes */
	mul a1, a1, t0

	/* align_offset */
	andi t2, a0, 0b111
	
	/* align pointer */
	andi a0, a0, ~0b111

	/* used to check if c was found past the end of string */
	li a3, 8

	/* if pointer is aligned skip to loop */
	beqz t2, .Lskip_start
	
	/* len = len - (8-align_offset) */
	addi t2, t2, -8
	add a2, a2, t2

	/* align first load */
	ld a4, (a0)
	
	xor a4, a4, a1

	/* fill bytes before b with non-zero */ 
	slli t2, t2, 3
	neg t2, t2
	srl t2, t0, t2
	or a4, a4, t2

	/* has_zero */
	not t2, a4
	sub a4, a4, t0
	and a4, a4, t2
	and a4, a4, t1	

	bnez a4, .Lfind_zero
	
	/* first iteration exit */
	addi a0, a0, 8

.Lskip_start:
	srli a5, a2, 3
	/* remaining bytes to check after the loop */
	andi a6, a2, 0b111

	/* while (b != last) */
	sltiu t2, a5, 2
	bnez t2, .Lskip_loop
.Lloop:
	ld a4, (a0)
	xor a4, a4, a1

	/* has_zero */
	not t2, a4
	sub a4, a4, t0
	and a4, a4, t2
	and a4, a4, t1

	bnez a4, .Lfind_zero
	
	ld a4, 8(a0)
	addi a0, a0, 8

	xor a4, a4, a1

	/* has_zero */
	not t2, a4
	sub a4, a4, t0
	and a4, a4, t2
	and a4, a4, t1

	bnez a4, .Lfind_zero
	
	addi a0, a0, 8
	addi a5, a5, -2

	sltiu t2, a5, 2
	beqz t2, .Lloop

.Lskip_loop:
	beqz a5, .Lunrolled_end
	ld a4, (a0)
	xor a4, a4, a1

	/* has_zero */
	not t2, a4
	sub a4, a4, t0
	and a4, a4, t2
	and a4, a4, t1

	bnez a4, .Lfind_zero
	
	addi a0, a0, 8
	#addi a5, a5, -1

.Lunrolled_end:

	/* ignore bytes past remaining in cccccccc */
	mv a3, a6
	
	/* 0 bytes remaining to check */
	beqz a3, .Lno_match

	ld a4, (a0)
	xor a4, a4, a1

	/* has_zero */
	not t2, a4
	sub a4, a4, t0
	and a4, a4, t2
	and a4, a4, t1
	
	bnez a4, .Lfind_zero

.Lno_match:
	li a0, 0
	ret

.Lfind_zero:
	/* isolate lowest set bit */
	neg t0, a4
	and a4, a4, t0

	li t0, 0x0001020304050607
	srli a4, a4, 7

	/* lowest set bit is 2^(8*k)
	 * multiplying by it shifts the idx array in t0 by k bytes to the left */
	mul	a4, a4, t0

	/* highest byte contains idx of first zero */
	srli a4, a4, 56
	
	/* bounds check */
	bgeu a4, a3, .Lno_match 
	
	add a0, a0, a4
	ret
