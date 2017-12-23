#include <kernel/yeeloong/env.h>
#include <kernel/port/string.h>
#include <kernel/port/panic.h>

char *env_get(char *var, char **env) {
	while(*env) {
		if(strncmp(var, *env, strlen(var)) == 0) {
			char *val = strchr(*env, '=');
			if(!val)
				panic("Missing value!");
			return val + 1;
		}
		env++;
	}
	panic("Nothing left in the environment!");
	/* compiler isn't happy about us not returning a value here, but
	 * we're not returning *at all*. let's give it a hint. */
	while(1);
}
