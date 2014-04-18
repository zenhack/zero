#include <kernel/rpi/act_led.h>
#include <kernel/rpi/morse.h>
#include <kernel/rpi/sleep.h>

#define DOT MORSE_UNIT
#define DASH (3 * MORSE_UNIT)

void light_for(uint32_t time) {
	act_led_on();
	sleep(time);
	act_led_off();
}

static uint32_t codes[256][7] = {
	['a'] = {DOT, DASH},
	['b'] = {DASH, DOT, DOT, DOT},
	['c'] = {DASH, DOT, DASH, DOT},
	['d'] = {DASH, DOT, DOT},
	['e'] = {DOT},
	['f'] = {DOT, DOT, DASH, DOT},
	['g'] = {DASH, DASH, DOT},
	['h'] = {DOT, DOT, DOT, DOT},
	['i'] = {DOT, DOT},
	['j'] = {DOT, DASH, DASH, DASH},
	['k'] = {DASH, DOT, DASH},
	['l'] = {DOT, DASH, DOT, DOT},
	['m'] = {DASH, DASH},
	['n'] = {DASH, DOT},
	['o'] = {DASH, DASH, DASH},
	['p'] = {DOT, DASH, DASH, DOT},
	['q'] = {DASH, DASH, DOT, DASH},
	['r'] = {DOT, DASH, DOT},
	['s'] = {DOT, DOT, DOT},
	['t'] = {DASH},
	['u'] = {DOT, DOT, DASH},
	['v'] = {DOT, DOT, DOT, DASH},
	['w'] = {DOT, DASH, DASH},
	['x'] = {DASH, DOT, DOT, DASH},
	['y'] = {DASH, DOT, DASH, DASH},
	['z'] = {DASH, DASH, DOT, DOT},

	['1'] = {DOT, DASH, DASH, DASH, DASH},
	['2'] = {DOT, DOT, DASH, DASH, DASH},
	['3'] = {DOT, DOT, DOT, DASH, DASH},
	['4'] = {DOT, DOT, DOT, DOT, DASH},
	['5'] = {DOT, DOT, DOT, DOT, DOT},
	['6'] = {DASH, DOT, DOT, DOT, DOT},
	['7'] = {DASH, DASH, DOT, DOT, DOT},
	['8'] = {DASH, DASH, DASH, DOT, DOT},
	['9'] = {DASH, DASH, DASH, DASH, DOT},
	['0'] = {DASH, DASH, DASH, DASH, DASH},
};

void morse_write(char *s, uintptr_t len) {
	while(len) {
		if(*s == ' ') {
			sleep(4 * MORSE_UNIT);
		} else {
			int i;
			for(i = 0; codes[(int)*s][i]; i++) {
				light_for(codes[(int)*s][i]);
				sleep(MORSE_UNIT);
			}
			sleep(2 * MORSE_UNIT);
		}
		s++;
		len--;
	}
}
