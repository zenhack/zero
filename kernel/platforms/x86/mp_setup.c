#include <kernel/port/stdio.h>
#include <kernel/port/string.h>
#include <kernel/port/data.h>
#include <kernel/port/heap.h>
#include <kernel/port/mmio.h>
#include <kernel/port/panic.h>
#include <kernel/x86/acpi.h>
#include <kernel/x86/apic.h>
#include <kernel/x86/asm.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/regs.h>
#include <kernel/x86/apic_timer_setup.h>
#include <kernel/x86/mp_setup.h>

/* defined in boot.S. Entry point for application processors. */
extern void (*ap_start)(void);

mutex_t ap_stack_lock;
volatile List *ap_stacks;


/* Count the number of local apic structures in sdt, which must be a MADT.
 * Each such structure represents a logical CPU. */
static size_t count_local_apic_structs(acpi_SDT *sdt) {
	/* The SDT's header gives us the total length of the table. Each entry
	 * contains a length field for that entry as well. We iterate over the
	 * entries until we're at the end of the entire table. There's some
	 * pointer arithmetic happening here due to the fact that C can't
	 * describe variable-length structs. */
	size_t count = 0;
	size_t length_remaining = sdt->header.length;
	acpi_IntCtlEnt *next_ent = &sdt->body.madt.int_ctl_ents;
	length_remaining -= ((size_t)next_ent) - ((size_t)sdt);
	while(length_remaining > 0) {
		/* There are other types, e.g. IOAPICs, so we need to check that
		 * it's the right kind. */
		if(next_ent->type == ACPI_INT_CTL_LOCAL_APIC) {
			count++;
		}
		length_remaining -= next_ent->length;
		next_ent = (acpi_IntCtlEnt*)(((uintptr_t)next_ent) + next_ent->length);
	}
	return count;
}

/* Determine the number of logical CPUs in the system.
 *
 * We retrieve this information from the ACPI tables. */
static size_t get_cpu_count(void) {
	/* TODO: move more of this logic to acpi.c */
	acpi_RSDP *rsdp = acpi_find_rsdp();
	if(!rsdp) {
		panic("RSDP not found!\n");
	}

	acpi_SDT *rsdt = (acpi_SDT*)rsdp->v1.rsdt_addr;
	if(!acpi_verify_sdt(rsdt)) {
		panic("RSDT checksum invalid!\n");
	}
	size_t num_sdts = (rsdt->header.length - sizeof(acpi_SDTHeader)) / sizeof(acpi_SDT *);
	acpi_SDT **sdts = (acpi_SDT**)&rsdt->body.rsdt.sdts;
	size_t cpu_count = 0;
	for(size_t i = 0; i < num_sdts; i++) {
		if(!acpi_verify_sdt(sdts[i])) {
			char *sig = (char *)&sdts[i]->header.signature[0];
			printf("Bad checksum for SDT %d (signature \"%c%c%c%c\").\n",
				i, sig[0], sig[1], sig[2], sig[3]);
			panic("Bad SDT checksum!\n");
		}
		if(memcmp(sdts[i], "APIC", 4) == 0) {
			size_t apic_struct_count = count_local_apic_structs(sdts[i]);
			printf("Found MADT with %d apic structures.\n", apic_struct_count);
			cpu_count += apic_struct_count;
		}
	}
	return cpu_count;
}

static void do_ipi(uint32_t deliv_mode, uint8_t vector) {
	ApicICR icr;
	icr.lo.raw = get32(INT_COMMAND);
	icr.lo.dest_shorthand = ICR_IPI_ALL_BUT_SELF;
	icr.lo.deliv_mode = deliv_mode;
	icr.lo.level = ICR_ASSERT;
	icr.lo.vector = vector;
	put32(INT_COMMAND, icr.lo.raw);
}

static void send_init(void) {
	do_ipi(ICR_DELIV_INIT, 0); /* vector is ignored here. */
}

static void send_sipi(uint8_t vector) {
	do_ipi(ICR_DELIV_STARTUP, vector);
}

void mp_setup(void) {
	printf("Bringing up extra CPUs...\n");
	uintptr_t ap_start_addr = (uintptr_t)&ap_start;
	if(ap_start_addr % 4096 != 0) {
		panic("BUG: ap_start() is not aligned on a page boundary!");
	}
	uintptr_t ap_start_page_num = ap_start_addr / 4096;
	if(ap_start_page_num > 0xff) {
		panic("BUG: ap_start() is located outside of low memory!");
	}
	size_t num_cpus = get_cpu_count();
	printf("CPU count: %d\n", num_cpus);

	/* allocate stacks. The -1 is because we don't need a new stack for the
	 * BSP. */
	size_t buf_size = 4096 * (num_cpus - 1);
	void *buf = (List *)kalloc_align(buf_size, 4096);
	ap_stacks = sew_list(buf, buf_size, 4096);


	printf("Sending init...\n");
	send_init();
	printf("Sending sipi...\n");
	/* TODO: we ought to be putting a more explicit wait here; intel
	 * suggests 10 ms. It seems to be working though (printf probably takes
	 * long enough). */
	send_sipi((uint8_t)ap_start_page_num);
	while(1);
}
