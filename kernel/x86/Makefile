
CC=cc -m32
LD=ld -melf_i386

LDFLAGS += -T$(TOP)/kernel/x86/link.ld

$(TARG)_OFILES += \
	$(TOP)/kernel/x86/boot.o\
	$(TOP)/kernel/x86/apic.o\
	$(TOP)/kernel/x86/arch_main.o\
	$(TOP)/kernel/x86/cpuid.o\
	$(TOP)/kernel/x86/gdt.o\
	$(TOP)/kernel/x86/gdt_load.o\
	$(TOP)/kernel/x86/idt.o\
	$(TOP)/kernel/x86/isr.o\
	$(TOP)/kernel/x86/isr_stub.o\
	$(TOP)/kernel/x86/lidt.o\
	$(TOP)/kernel/x86/serial.o\
	$(TOP)/kernel/x86/text_console.o\
	$(TOP)/kernel/x86/cothread.o\
	$(TOP)/kernel/x86/cothread_asm.o\
	$(TOP)/kernel/x86/paging_asm.o
