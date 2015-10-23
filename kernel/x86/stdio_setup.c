#include <kernel/port/muxwriter.h>
#include <kernel/port/stdio.h>

#include <kernel/x86/serial.h>
#include <kernel/x86/stdio_setup.h>
#include <kernel/x86/text_console.h>

static MuxWriter mux_writer;
static SerialPort com1;
static FILE console;

void stdio_init(void) {
	/* direct output to both the serial line and video console. */
	serial_init(COM1, &com1);
	text_console_init(&console);
	muxwriter_init(&mux_writer, (FILE*)&com1, &console);
	stdout = (FILE*)&mux_writer;
}
