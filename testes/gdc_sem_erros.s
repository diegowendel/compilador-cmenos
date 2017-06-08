	.file	"gdc_sem_erros.c"
	.comm	globalVar,4,4
	.text
	.globl	input
	.type	input, @function
input:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	input, .-input
	.globl	output
	.type	output, @function
output:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	output, .-output
	.globl	gdc
	.type	gdc, @function
gdc:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$0, -8(%rbp)
	jne	.L4
	movl	-4(%rbp), %eax
	jmp	.L5
.L4:
	movl	-4(%rbp), %eax
	cltd
	idivl	-8(%rbp)
	movl	-8(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	gdc
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	gdc, .-gdc
	.globl	main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$3, -60(%rbp)
	call	input
	movl	%eax, -68(%rbp)
	call	input
	movl	%eax, -56(%rbp)
	movl	-56(%rbp), %edx
	movl	-68(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	gdc
	movl	%eax, %edi
	call	output
	movl	$0, -64(%rbp)
	movl	$1, -68(%rbp)
	jmp	.L7
.L8:
	addl	$1, -64(%rbp)
	movl	-52(%rbp), %eax
	cltq
	movl	-64(%rbp), %edx
	movl	%edx, -48(%rbp,%rax,4)
	addl	$1, -68(%rbp)
.L7:
	cmpl	$10, -68(%rbp)
	jle	.L8
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L10
	call	__stack_chk_fail
.L10:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
