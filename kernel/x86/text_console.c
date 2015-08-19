#include <kernel/x86/text_console.h>
#include <kernel/x86/portio.h>
#include <kernel/arch/lock.h>

#define VGA_CNTL 0x3d4
#define VGA_DATA 0x3d5

#define CURSOR_HI_BYTE 14
#define CURSOR_LO_BYTE 15

static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;

static uint16_t *video_memory = (uint16_t*)0xb8000;

/** video_lock must be held when writing to the video memory,
 * or adjusting the cursor position. */
static mutex_t video_lock;

#define BLACK     0
#define BLUE      1
#define GREEN     2
#define CYAN      3
#define RED       4
#define MAGENTA   5
#define BROWN     6
#define L_GRAY    7
#define D_GRAY    8
#define L_BLUE    9
#define L_GREEN   10
#define L_CYAN    11
#define L_RED     12
#define L_MAGENTA 13
#define L_BROWN   14
#define WHITE     15

#define FG(color) ((color) << 8)
#define BG(color) ((color) << 12)

#define COLS 80
#define ROWS 25

#define TABSTOP 8

#define COLOR (FG(WHITE) | BG(BLACK))

#define BLANK (COLOR | ' ')

#define CELL(X,Y) ((Y) * COLS + (X))

static void update_hardware_cursor(void) {
	uint16_t cursor_location = cursor_y * 80 + cursor_x;
	out8(VGA_CNTL, CURSOR_HI_BYTE);
	out8(VGA_DATA, cursor_location >> 8);
	out8(VGA_CNTL, CURSOR_LO_BYTE);
	out8(VGA_DATA, cursor_location);
}

/* scrolls the text on the screen up one line, if needed. */
static void scroll(void) {
	if(cursor_y >= ROWS) {
		int i;
		for(i = 0; i < (ROWS - 1) * COLS; i++) {
			video_memory[i] = video_memory[i + COLS];
		}

		for(i = (ROWS - 1) * COLS; i < ROWS * COLS; i++) {
			video_memory[i] = BLANK;
		}
		cursor_y = ROWS - 1;
	}
}

static size_t text_console_write(FILE *stream, void *buf, size_t len) {
	char *s = (char*)buf;
	size_t left = len;
	wait_aquire(&video_lock);
	while(left) {
		switch(*s) {
		case '\r':
			cursor_x = 0;
			break;
		case '\n':
			cursor_x = 0;
			cursor_y++;
			break;
		case '\t':
			cursor_x %= TABSTOP;
			cursor_x += TABSTOP;
			break;
		case '\b':
			cursor_x--;
			break;
		default:
			if(*s >= ' ' && *s <= '~') {
				video_memory[CELL(cursor_x,cursor_y)] = COLOR | *s;
				cursor_x++;
			}
			break;
		}
		if(cursor_x > COLS) {
			cursor_x = 0;
			cursor_y++;
		}
		s++;
		left--;
		scroll();
		update_hardware_cursor();
	}
	release(&video_lock);
	return len - left;
}

void text_console_init(FILE *stream) {
	int i;
	for(i = 0; i < ROWS * COLS; i++) {
		video_memory[i] = BLANK;
	}
	update_hardware_cursor();
	stream->write = text_console_write;
}
