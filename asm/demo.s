	.section .data
x: 	.int 	4
y:	.int 	5
	.comm	z 4
fmt:	.string "%d + %d =%d\n"
fmt2:	.string "sum=%d\n"

	.section .text
main:
	movl x,%eax
	addl y,%eax
	movl %eax,z

	pushl z
	pushl y
	pushl x
	pushl $fmt
	call printf
	
	pushl $50
	pushl $60
        call myplus 
	pushl %eax
	pushl $fmt2
	call printf

	addl $32 ,%esp
	ret
	.global main
