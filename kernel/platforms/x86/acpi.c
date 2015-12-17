#include <kernel/port/string.h>
#include <kernel/port/units.h>
#include <kernel/x86/acpi.h>
#include <stddef.h>


/**
 * The logic for finding the RSDP (RSDP_*_SEARCH, verify_checksum,
 * acpi_find_rsdp) is pulled from [acpi5/5.2.5.1] and [acpi5/5.2.5.3].
 *
 * Short version: look for something starting with "RSD PTR " on a 16-byte
 * boundary, and see if the rsdp checksum works out. Should be in the given
 * memory range, or the first 1KiB of the EBDA.
 */

#define RSDP_START_SEARCH 0xe0000
#define RSDP_END_SEARCH 0xfffff

/**
 * Location in memory (in the BIOS data area) of a pointer to the extended BIOS
 * data area (EBDA), shifted down by 4 bits.
 */
#define EBDA_PTR ((uintptr_t *)0x40e)

/* Return true if the checksum for the RSDP structure validates, else false. */
static int verify_checksum(uint8_t *rsdp) {
	uint8_t checksum = 0;
	for(int i = 0; i < 20; i++) {
		checksum += rsdp[i];
	}
	return checksum == 0;
}

/** look for the rsdp in the given address range. Return a pointer to it, or
 * NULL if not found. */
acpi_RSDP *acpi_find_rsdp_range(uintptr_t start, uintptr_t end) {
	uintptr_t cursor = start;
	while(cursor <= end) {
		if(memcmp((void *)cursor, "RSD PTR ", 8) == 0
				&& verify_checksum((uint8_t *)cursor)) {
			return (acpi_RSDP *)cursor;
		}
		cursor += 16;
	}
	return NULL;
}

acpi_RSDP *acpi_find_rsdp(void) {
	acpi_RSDP *ret;
	ret = acpi_find_rsdp_range(RSDP_START_SEARCH, RSDP_END_SEARCH);
	if(ret != NULL) {
		return ret;
	}
	uintptr_t ebda = *EBDA_PTR;
	ebda <<= 4;
	return acpi_find_rsdp_range(ebda, ebda + KIBI);
}
