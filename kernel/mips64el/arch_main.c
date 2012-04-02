#include <kernel/mips64el/pmon2k.h>

int arch_main(int argc, char **argv, char **env, callvectors *cv) {
	cv->printf("Booted!\n");
	return 0;
}
