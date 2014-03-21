#include <kernel/yeeloong/pmon2k.h>
#include <kernel/yeeloong/env.h>
#include <kernel/port/string.h>
#include <kernel/port/stdio.h>
#include <kernel/yeeloong/pmon2k_stdio.h>
#include <kernel/yeeloong/exception.h>
#include <kernel/yeeloong/instruction.h>

int arch_main(int argc, char **argv, char **env, callvectors *cv) {
	FILE console;
	uint32_t *tlb_refill_vec;

	cv->printf("Booted!\n");

	/* Insert a jump instruction to the refill handler at the right vector. */
	tlb_refill_vec = (uint32_t*)BEV0_TLB_REFILL_VEC;
	*tlb_refill_vec = jump_instr((uintptr_t)tlb_refill_vec, (uintptr_t)&tlb_refill);

	init_pmon_stdout(&console, cv);

	printf("Our own printf works!\n");	
	return 0;

/*	val = env_get("systype", env);
	if(strcmp(val, "Bonito") == 0)
		cv->printf("systype is Bonito, as expected.\n");
	else
		cv->printf("Error, expected systype Bonito, but found '%s'.\n", val);
	return 0; */
}
