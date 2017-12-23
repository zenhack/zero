#include <kernel/yeeloong/pmon2k.h>
#include <kernel/yeeloong/env.h>
#include <kernel/port/string.h>
#include <kernel/port/stdio.h>
#include <kernel/yeeloong/pmon2k_stdio.h>
#include <kernel/yeeloong/exception.h>
#include <kernel/yeeloong/instruction.h>

int arch_main(int argc, char **argv, char **env, callvectors *cv) {
	PMONFILE console;
//	uint32_t *tlb_refill_vec;

	cv->printf("Booted!\n");
	
	init_pmon_stdout(&console, cv);
	stdout = (FILE *)&console;

	/* Insert a jump instruction to the refill handler at the right vector. */
//	tlb_refill_vec = (uint32_t*)BEV0_TLB_REFILL_VEC;
//	*tlb_refill_vec = jump_instr((uintptr_t)tlb_refill_vec, (uintptr_t)&tlb_refill);

	char str[] = "our own printf works!\n";
	printf("Hello, %s", str);

	printf("hello, \nworld!\n");

	while(*env) {
		printf("%d\n", (int)env);
		printf("%s\n", *env);
		env++;
	}

	return 0;
	printf("%s", cv->gets("systype"));

	char *val = env_get("systype", env);
	if(strcmp(val, "Bonito") == 0)
		cv->printf("systype is Bonito, as expected.\n");
	else
		cv->printf("Error, expected systype Bonito, but found '%s'.\n", val);
	return 0;
}
