#include <kernel/yeeloong/env.h>
#include <kernel/port/string.h>

char *env_get(char *var, char **env) {
	while(*env) {
		if(strncmp(var, *env, strlen(var)) == 0) {
			char *val = strchr(*env, '=');
			/* FIXME: we really should use panic here, but we don't have stdout set up yet. */
			if(!val)
				while(1);
			return val + 1;
		}
		env++;
	}
	while(1); /* FIXME: another place for panic. */
}
