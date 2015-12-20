#ifndef KERNEL_X86_ACPI_H
#define KERNEL_X86_ACPI_H

#include <stdint.h>

typedef struct acpi_RSDPv1 acpi_RSDPv1;
typedef struct acpi_RSDP acpi_RSDP;
typedef struct acpi_SDTHeader acpi_SDTHeader;

struct acpi_RSDPv1 {
	/* Fields available in acpi 1.0. */
	uint8_t signature[8];
	uint8_t checksum;
	uint8_t oemid[6];
	uint8_t revision;
	uint32_t rsdt_addr;
}__attribute__((packed));
_Static_assert(sizeof(acpi_RSDPv1) == 20, "acpi_RSDPv1 is the wrong size.");

struct acpi_RSDP {
	acpi_RSDPv1 v1;
	/* Fields introduced after acpi 1.0: */
	uint32_t length;
	uint64_t xsdt_addr;
	uint8_t extended_checksum;
	uint8_t _reserved[3];
}__attribute__((packed));
_Static_assert(sizeof(acpi_RSDP) == 36, "acpi_RSDP is the wrong size.");

struct acpi_SDTHeader {
	uint8_t signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	uint8_t oemid[6];
	uint8_t oem_table_id[8];
	uint32_t oem_revision;
	uint32_t creator_id;
	uint32_t creator_revision;
}__attribute__((packed));
_Static_assert(sizeof(acpi_SDTHeader) == 36, "acpi_SDTHeader is the wrong size.");

/* Locate the RSDP in memory, returning its address. Returns NULL if the RSDP
 * cannot be found. The BIOS's memory must be identity-mapped for this to work. */
acpi_RSDP *acpi_find_rsdp(void);

#endif
