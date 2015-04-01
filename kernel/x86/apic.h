#ifndef KERNEL_X86_APIC_H
#define KERNEL_X86_APIC_H

#include <stdint.h>

/* Local apic register map, as defined in the intel manual, volume 3. */
#define LOCAL_APIC         0xfee00000
#define LOCAL_APIC_ID      0xfee00020
#define LOCAL_APIC_VERSION 0xfee00030
#define TASK_PRIOR         0xfee00080
#define ARBITRATE_PRIOR    0xfee00090
#define PROCESSOR_PRIOR    0xfee000a0
#define EOI                0xfee000b0
#define REMOTE_READ        0xfee000c0
#define LOGIC_DEST         0xfee000d0
#define DEST_FORMAT        0xfee000e0
#define SPUR_INT_VEC       0xfee000f0
#define IN_SERVICE         0xfee00100
#define TRIGGER_MODE       0xfee00180
#define INT_REQ            0xfee00200
#define ERR_STATUS         0xfee00280
#define LVT_CMCI           0xfee002f0
#define INT_COMMAND        0xfee00300
#define LVT_TIMER          0xfee00320
#define LVT_THERMAL        0xfee00330
#define LVT_PERF_MON       0xfee00340
#define LVT_LINT0          0xfee00350
#define LVT_LINT1          0xfee00360
#define LVT_ERR            0xfee00370
#define INITIAL_COUNT      0xfee00380
#define CURRENT_COUNT      0xfee00390
#define DIVIDE_CONF        0xfee003e0

#define IA32_APIC_BASE     0x1b

/* flags for SPUR_INT_VEC */
#define APIC_SOFTWARE_ENABLE (1<<8)

typedef union LVTEnt LVTEnt;

union LVTEnt {
	struct {
		uint32_t vector : 8;
		uint32_t deliv_mode : 3;
		uint32_t res11 : 1;
		uint32_t deliv_status : 1;
		uint32_t int_input_pin_polarity : 1;
		uint32_t remote_irr : 1;
		uint32_t trigger_mode : 1;
		uint32_t masked : 1;
		uint32_t timer_mode : 2;
		uint32_t res19_31 : 13;
	} v;
	uint32_t raw;
};

int have_apic(void);
void enable_local_apic(void);
uint32_t get_local_apic_id(void);

#endif
