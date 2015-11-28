#ifndef KERNEL_X86_ACPI_H
#define KERNEL_X86_ACPI_H

#include <stdint.h>

typedef struct acpi_RSDP acpi_RSDP;

struct acpi_RSDP {
	uint8_t signature[8];
	uint8_t checksum;
	uint8_t oemid[6];
	uint8_t revision;
	uint32_t rsdt_addr;
	/* The above is all in acpi 1.0, the rest is later revisions only: */
	uint32_t length;
	uint64_t xsdt_addr;
	uint8_t extended_checksum;
	uint8_t _reserved[3];
}__attribute__((packed));

/* Locate the RSDP in memory, returning its address. Returns NULL if the RSDP
 * cannot be found. The BIOS's memory must be identity-mapped for this to work. */
acpi_RSDP *acpi_find_rsdp(void);

#endif
