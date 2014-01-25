.globl init_thread
init_thread:
	mov 4(%esp), %eax // new stack pointer
	mov 8(%esp), %edx // entry point

	// Save callee-saved registers. Without this, yield will not have
	// anything to restore when it comes back to us.
	push %ebx
	push %ebp
	push %esi
	push %edi

	// Switch to the new thread's stack (saving the old one):
	mov %esp, %ecx
	mov %eax, %esp

	// Save a copy of our entry point:
	push %edx

	// Yeild to our original caller
	push %ecx
	call yield
	// Restore our saved entry point:
	pop %edx
	// The thread's been started for the first time; restore our entry
	// point and pass the yielding thread to it.
	pop %edx
	push %eax
	call *%edx
	// The entry point should never return. If it does, panic.
	push $cothread_bad_return
	call panic
	

cothread_bad_return:
.ascii "Return from cothread entry point!\0"
	

// void *yield(void *to);
.globl yield
yield:
	// Grab the pointer `to`; This is the thread we're switching to.
	mov 4(%esp), %edx
	// Save callee-saved registers:
	push %ebx
	push %ebp
	push %esi
	push %edi
	// We want to return the stack pointer for this thread to the resumed
	// thread, so let's take care of that now, before we lose track of it.
	mov %esp, %eax
	// Load the new thread's stack pointer:
	mov %edx, %esp
	// restore callee-saved regisers:
	pop %edi
	pop %esi
	pop %ebp
	pop %ebx
	// Return to the *new* thread:
	ret
