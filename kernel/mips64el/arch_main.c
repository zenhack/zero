#include <kernel/mips64el/pmon2k.h>
#include <kernel/mips64el/env.h>
#include <kernel/port/string.h>
#include <kernel/port/stdio.h>
#include <kernel/mips64el/pmon2k_stdio.h>

int arch_main(int argc, char **argv, char **env, callvectors *cv) {
	FILE console;
	
	cv->printf("Booted!\n");
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
