#ifndef KERNEL_MIPS64EL_INSTRUCTION_H
#define KERNEL_MIPS64EL_INSTRUCTION_H

/* routines and constants for constructing mips instructions on the fly. */

#define OPCODE_J (1<<1)
#define OPCODE_OFFSET 26

static inline uint32_t jump_instr(uintptr_t from, uintptr_t to) {
	return (OPCODE_J<<OPCODE_OFFSET) | ((to - from) >> 2);
}

#endif
