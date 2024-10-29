/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2024 Strahinja Stanisic <strajabot@FreeBSD.org>
 */
 
/*
 * a0 - char* restrict dstv
 * a1 - char* restrict srcv
 */
.globl stpcpy_riscv
.type stpcpy_riscv, @function
stpcpy_riscv:
    /* a0 - char* restrict dstv
     * a1 - char* restrict srcv
     * a2 - char iter1[8]
     * a4 - size_t dst_align_rem
     */
 
    andi a4, a0, 0b111
    beqz a4, .Lskip_dst_align

    /* byte one */
    lbu a2, 0(a1)
    addi a0, a0, 1
    addi a1, a1, 1
    sb a2, -1(a0)
    beqz a2, .Lret

    /* byte two */
    lbu a2, 0(a1)
    addi a0, a0, 1
    addi a1, a1, 1
    sb a2, -1(a0)
    beqz a2, .Lret
    
    /* byte three */
    lbu a2, 0(a1)
    addi a0, a0, 1
    addi a1, a1, 1
    sb a2, -1(a0)
    beqz a2, .Lret

    /* byte four */
    lbu a2, 0(a1)
    addi a0, a0, 1
    addi a1, a1, 1
    sb a2, -1(a0)
    beqz a2, .Lret

    /* byte five */
    lbu a2, 0(a1)
    addi a0, a0, 1
    addi a1, a1, 1
    sb a2, -1(a0)
    beqz a2, .Lret

    /* byte six */
    lbu a2, 0(a1)
    addi a0, a0, 1
    addi a1, a1, 1
    sb a2, -1(a0)
    beqz a2, .Lret
    
    /* byte seven */
    lbu a2, 0(a1)
    addi a0, a0, 1
    addi a1, a1, 1
    sb a2, -1(a0)
    beqz a2, .Lret

    addi a0, a0, 1
    addi a1, a1, 1

.Lskip_dst_align:
	/* t0 = 0x0101010101010101
	 * t1 = 0x8080808080808080
     * if we moved more bytes than necessary move pointers back to alignment
     */
	li t0, 0x01010101
    sub a1, a1, a4
 	slli t1, t0, 32
    sub a0, a0, a4
	or t0, t0, t1
    andi a2, a1, 0b111
	slli t1, t0, 7

    /* dst pointer is aligned here, check if src is aligned too and jump to faster routine */    
    beqz a2, .Lstpcpy8

    /* temp: src pointer is not 8byte aligned :sob: */

    /* todo: write a fast implementation of the loop bellow using bitshifts :sob: */

.Llsimple:
    lbu a2, 0(a1)
    addi a0, a0, 1
    addi a1, a1, 1
    sb a2, -1(a0)
    bnez a2, .Llsimple
    addi a0, a0, -1
    ret
    
.Lstpcpy8: 

    /* align to 16 bytes */
    ld a2, 0(a1)
    
    /* has_zero */
    not a3, a2
    sub a4, a2, t0
    and a3, a3, t1
    and a4, a4, a3
    
    bnez a4, .Lend
    
    sd a2, 0(a0)

    addi a1, a1, 8
    addi a0, a0, 8
    
    andi a1, a1, ~0b1111
    andi a0, a0, ~0b1111

.Llmain8_16:
    /* if we align src to 16 here, we can rearange the sequential code
     * ld -> sd -> ld -> sd into ld -> ld -> sd -> sd
     * the second ld can't PF if the first ld didn't PF
     */ 

    ld a2, 0(a1)
    ld a3, 8(a1)

    /* has_zero */
    not a4, a2
    not a5, a3
    sub t2, a2, t0
    sub t3, a3, t0
    and a4, a4, t1
    and a5, a5, t1 
    and a4, a4, t2
    and a5, a5, t3

    bnez a4, .Lend
    
    sd a2, 0(a0)

    mv a2, a3
    mv a4, a5
    addi a0, a0, 8
    addi a1, a1, 8
    bnez a5, .Lend
    
    sd a3, 0(a0)
    
    addi a0, a0, 8
    addi a1, a1, 8
    j .Llmain8_16

.Lend:
    andi a3, a4, 0xff
    sb a2, 0(a0)
    srli a4, a4, 8
    addi a1, a1, 1
    srli a2, a2, 8
    addi a0, a0, 1
    bnez a3, .Lret

    andi a3, a4, 0xff
    sb a2, 0(a0)
    srli a4, a4, 8
    addi a1, a1, 1
    srli a2, a2, 8
    addi a0, a0, 1
    bnez a3, .Lret

    andi a3, a4, 0xff
    sb a2, 0(a0)
    srli a4, a4, 8
    addi a1, a1, 1
    srli a2, a2, 8
    addi a0, a0, 1
    bnez a3, .Lret

    andi a3, a4, 0xff
    sb a2, 0(a0)
    srli a4, a4, 8
    addi a1, a1, 1
    srli a2, a2, 8
    addi a0, a0, 1
    bnez a3, .Lret

    andi a3, a4, 0xff
    sb a2, 0(a0)
    srli a4, a4, 8
    addi a1, a1, 1
    srli a2, a2, 8
    addi a0, a0, 1
    bnez a3, .Lret

    andi a3, a4, 0xff
    sb a2, 0(a0)
    srli a4, a4, 8
    addi a1, a1, 1
    srli a2, a2, 8
    addi a0, a0, 1
    bnez a3, .Lret

    andi a3, a4, 0xff
    sb a2, 0(a0)
    srli a4, a4, 8
    addi a1, a1, 1
    srli a2, a2, 8
    addi a0, a0, 1
    bnez a3, .Lret

    addi a0, a0, 1
    andi a3, a4, 0xff
    sb a2, -1(a0)

.Lret:
    addi a0, a0, -1
    ret