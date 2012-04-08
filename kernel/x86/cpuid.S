
.globl cpuid
cpuid:
	pushl %edi
	movl 8(%esp), %edi
	movl (%edi), %eax
	movl 8(%edi), %ecx
	cpuid
	movl %eax, (%edi)
	movl %ebx, 4(%edi)
	movl %ecx, 8(%edi)
	movl %edx, 12(%edi)
	popl %edi
	ret
	
