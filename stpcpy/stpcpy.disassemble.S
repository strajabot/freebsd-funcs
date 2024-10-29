   10540:	40a00733          	neg	a4,a0
   10544:	8b1d                	andi	a4,a4,7
   10546:	cb65                	beqz	a4,10636 <stpcpy_riscv+0xf6>
   10548:	0075f693          	andi	a3,a1,7
   1054c:	99e1                	andi	a1,a1,-8
   1054e:	ff868793          	addi	a5,a3,-8
   10552:	6190                	ld	a2,0(a1)
   10554:	973e                	add	a4,a4,a5
   10556:	8d1d                	sub	a0,a0,a5
   10558:	00469313          	slli	t1,a3,0x4
   1055c:	068e                	slli	a3,a3,0x3
   1055e:	0086f793          	andi	a5,a3,8
   10562:	00d65633          	srl	a2,a2,a3
   10566:	00000397          	auipc	t2,0x0
   1056a:	86b2                	mv	a3,a2
   1056c:	00f65633          	srl	a2,a2,a5
   10570:	0087c793          	xori	a5,a5,8
   10574:	00f6d6b3          	srl	a3,a3,a5
   10578:	006387b3          	add	a5,t2,t1
   1057c:	05a1                	addi	a1,a1,8
   1057e:	01c78067          	jr	28(a5)
   10582:	0ff67793          	zext.b	a5,a2
   10586:	fec50c23          	sb	a2,-8(a0)
   1058a:	0c078b63          	beqz	a5,10660 <stpcpy_riscv+0x120>
   1058e:	01065613          	srli	a2,a2,0x10
   10592:	0ff6f793          	zext.b	a5,a3
   10596:	fed50ca3          	sb	a3,-7(a0)
   1059a:	0c078563          	beqz	a5,10664 <stpcpy_riscv+0x124>
   1059e:	0106d693          	srli	a3,a3,0x10
   105a2:	0ff67793          	zext.b	a5,a2
   105a6:	fec50d23          	sb	a2,-6(a0)
   105aa:	0a078f63          	beqz	a5,10668 <stpcpy_riscv+0x128>
   105ae:	01065613          	srli	a2,a2,0x10
   105b2:	0ff6f793          	zext.b	a5,a3
   105b6:	fed50da3          	sb	a3,-5(a0)
   105ba:	0a078963          	beqz	a5,1066c <stpcpy_riscv+0x12c>
   105be:	0106d693          	srli	a3,a3,0x10
   105c2:	0ff67793          	zext.b	a5,a2
   105c6:	fec50e23          	sb	a2,-4(a0)
   105ca:	0a078363          	beqz	a5,10670 <stpcpy_riscv+0x130>
   105ce:	01065613          	srli	a2,a2,0x10
   105d2:	0ff6f793          	zext.b	a5,a3
   105d6:	fed50ea3          	sb	a3,-3(a0)
   105da:	08078d63          	beqz	a5,10674 <stpcpy_riscv+0x134>
   105de:	0106d693          	srli	a3,a3,0x10
   105e2:	0ff67793          	zext.b	a5,a2
   105e6:	fec50f23          	sb	a2,-2(a0)
   105ea:	08078763          	beqz	a5,10678 <stpcpy_riscv+0x138>
   105ee:	01065613          	srli	a2,a2,0x10
   105f2:	0ff6f793          	zext.b	a5,a3
   105f6:	fed50fa3          	sb	a3,-1(a0)
   105fa:	08078163          	beqz	a5,1067c <stpcpy_riscv+0x13c>
   105fe:	0106d693          	srli	a3,a3,0x10
   10602:	02e05a63          	blez	a4,10636 <stpcpy_riscv+0xf6>
   10606:	953a                	add	a0,a0,a4
   10608:	6190                	ld	a2,0(a1)
   1060a:	95ba                	add	a1,a1,a4
   1060c:	070e                	slli	a4,a4,0x3
   1060e:	86b2                	mv	a3,a2
   10610:	40e007b3          	neg	a5,a4
   10614:	8b21                	andi	a4,a4,8
   10616:	03f7f793          	andi	a5,a5,63
   1061a:	00e65633          	srl	a2,a2,a4
   1061e:	00874713          	xori	a4,a4,8
   10622:	00179293          	slli	t0,a5,0x1
   10626:	00000797          	auipc	a5,0x0
   1062a:	00e6d6b3          	srl	a3,a3,a4
   1062e:	9796                	add	a5,a5,t0
   10630:	4701                	li	a4,0
   10632:	f5c78067          	jr	-164(a5) # 10582 <stpcpy_riscv+0x42>
   10636:	953a                	add	a0,a0,a4
   10638:	95ba                	add	a1,a1,a4
   1063a:	0005c603          	lbu	a2,0(a1)
   1063e:	00c50023          	sb	a2,0(a0)
   10642:	ce11                	beqz	a2,1065e <stpcpy_riscv+0x11e>
   10644:	00158613          	addi	a2,a1,1
   10648:	00064683          	lbu	a3,0(a2)
   1064c:	00150593          	addi	a1,a0,1
   10650:	00d500a3          	sb	a3,1(a0)
   10654:	0605                	addi	a2,a2,1
   10656:	852e                	mv	a0,a1
   10658:	fae5                	bnez	a3,10648 <stpcpy_riscv+0x108>
   1065a:	852e                	mv	a0,a1
   1065c:	8082                	ret
   1065e:	8082                	ret
   10660:	1561                	addi	a0,a0,-8
   10662:	8082                	ret
   10664:	1565                	addi	a0,a0,-7
   10666:	8082                	ret
   10668:	1569                	addi	a0,a0,-6
   1066a:	8082                	ret
   1066c:	156d                	addi	a0,a0,-5
   1066e:	8082                	ret
   10670:	1571                	addi	a0,a0,-4
   10672:	8082                	ret
   10674:	1575                	addi	a0,a0,-3
   10676:	8082                	ret
   10678:	1579                	addi	a0,a0,-2
   1067a:	8082                	ret
   1067c:	157d                	addi	a0,a0,-1
   1067e:	8082                	ret
