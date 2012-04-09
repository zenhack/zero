
.globl cpuid
cpuid:
	pushl %edi
	pushl %ebx
	movl 12(%esp), %edi
	movl (%edi), %eax
	movl 8(%edi), %ecx
	cpuid
	movl %eax, (%edi)
	movl %ebx, 4(%edi)
	movl %ecx, 8(%edi)
	movl %edx, 12(%edi)
	popl %ebx
	popl %edi
	ret
	
