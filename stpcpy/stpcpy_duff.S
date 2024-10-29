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
 
    neg a4, a0
    andi a4, a4, 0b111
    beqz a4, .Lskip_dst_align
 
    /* a5 - size_t neg_store_count
     * a3 - size_t src_align_off
     * t1 - size_t table_offset
     * t2 - void* jump_reg
     */
    andi a3, a1, 0b111
    andi a1, a1, ~0b111
 
    addi a5, a3, -8
    ld a2, (a1)
    add a4, a4, a5 
    sub a0, a0, a5

    slli t1, a3, 4
    slli a3, a3, 3

    andi a5, a3, 0b1000
    srl a2, a2, a3

1:  auipc t2, %pcrel_hi(.Lduff_align)
    /* a3 - char iter2[8] */ 
    mv a3, a2
    srl a2, a2, a5 
    /* a5 - size_t align_off_odd */ 
    xori a5, a5, 0b1000
    srl a3, a3, a5
    /* a5 - void* jump_reg */
    add a5, t2, t1
    addi a1, a1, 8
    jr a5, %pcrel_lo(1b)
 
    .option push
    .option arch, -c
    .option norelax
.Lduff_align:
 
    /* a5 - byte */
    andi a5, a2, 0xff
    sb a2, -8(a0)
    beqz a5, .Ldst_align_ret8
    srli a2, a2, 16
 
    andi a5, a3, 0xff
    sb a3, -7(a0)
    beqz a5, .Ldst_align_ret7
    srli a3, a3, 16
 
    andi a5, a2, 0xff
    sb a2, -6(a0)
    beqz a5, .Ldst_align_ret6
    srli a2, a2, 16
 
    andi a5, a3, 0xff
    sb a3, -5(a0)
    beqz a5, .Ldst_align_ret5
    srli a3, a3, 16
 
    andi a5, a2, 0xff
    sb a2, -4(a0)
    beqz a5, .Ldst_align_ret4
    srli a2, a2, 16
 
    andi a5, a3, 0xff
    sb a3, -3(a0)
    beqz a5, .Ldst_align_ret3
    srli a3, a3, 16
 
    andi a5, a2, 0xff
    sb a2, -2(a0)
    beqz a5, .Ldst_align_ret2
    srli a2, a2, 16
 
    andi a5, a3, 0xff
    sb a3, -1(a0)
    beqz a5, .Ldst_align_ret1
    srli a3, a3, 16
 
    .option pop
 
    /* unpredictable, maybe remove somehow */
    blez a4, .Lskip_dst_align
 
    /* src was more unaligned than dst */
 
    /* t0 - size_t table_offset
     * a5 - size_t align_rem_odd
     */
    add a0, a0, a4
    ld a2, (a1)
    add a1, a1, a4
    /* a4 - size_t dst_align_rem_bits */
    slli a4, a4, 3
    mv a3, a2 
    neg a5, a4
    andi a4, a4, 0b1000
    andi a5, a5, 0b111111
    srl a2, a2, a4    
    /* a4 - size_t align_off_odd */ 
    xori a4, a4, 0b1000
    slli t0, a5, 1
    /* a5 - void* jump_reg */
1:  auipc a5, %pcrel_hi(.Lduff_align)
    srl a3, a3, a4
    add a5, a5, t0
    /* a4 - size_t dst_align_rem */
    li a4, 0
    jr a5, %pcrel_lo(1b)
 
 
.Lskip_dst_align:

	/* t0 = 0x0101010101010101
	 * t1 = 0x8080808080808080
     * if we moved more bytes than necessary move pointers back to alignment
     */
	li t0, 0x01010101
    add a1, a1, a4
 	slli t1, t0, 32
    add a0, a0, a4
	or t0, t0, t1
    andi a2, a1, 0b111
	slli t1, t0, 7

    /* dst pointer is aligned here, check if src is aligned too and jump to faster routine */    
    beqz a2, .Lstpcpy8

    /* temp: src pointer is not 8byte aligned :sob: */

    /* todo: remove baseline impl from here */

.Llsimple:
    lbu a2, 0(a1)
    addi a0, a0, 1
    addi a1, a1, 1
    sb a2, -1(a0)
    bnez a2, .Llsimple
    addi a0, a0, -1
    ret
 
.Ldst_align_ret8:
    addi a0, a0, -8
    ret
.Ldst_align_ret7:
    addi a0, a0, -7
    ret
.Ldst_align_ret6:
    addi a0, a0, -6
    ret
.Ldst_align_ret5:
    addi a0, a0, -5
    ret
.Ldst_align_ret4:
    addi a0, a0, -4
    ret
.Ldst_align_ret3:
    addi a0, a0, -3
    ret
.Ldst_align_ret2:
    addi a0, a0, -2
    ret
.Ldst_align_ret1:
    addi a0, a0, -1
    ret

.Lstpcpy8: 
    /* if we align src to 16 here, we can rearange the sequential code
     * ld -> sd -> ld -> sd into ld -> ld -> sd -> sd
     * the second ld can't PF if the first ld didn't PF
     */ 

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
    beqz a3, .Lend
    addi a0, a0, -1
    ret

    /* todo: final byte store */