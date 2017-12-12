#myplus.s


.section .text

myplus: 	 #when calling myplus ,the computer stolens to pushl %eip for currently return to original place + 1(%eip inc automatic)
	pushl %ebp #preserve caller 's stackframe
	movl %esp,%ebp #because %esp still be changed,we use %ebp to as base address to get params 
	movl 8(%ebp),%eax #4 + 4
	addl 12(%ebp),%eax
	popl %ebp
	ret 		# equal to popl %eip

.global myplus
