#include <kernel/port/string.h>
#include <kernel/x86/acpi.h>
#include <stddef.h>


/**
 * The logic for finding the RSDP (RSDP_*_SEARCH, verify_checksum,
 * acpi_find_rsdp) is pulled from [acpi5/5.2.5.1] and [acpi5/5.2.5.3].
 *
 * Short version: look for something starting with "RSD PTR " on a 16-byte
 * boundary, and see if the rsdp checksum works out. Should be in the given
 * memory range.
 */

#define RSDP_START_SEARCH 0xe0000
#define RSDP_END_SEARCH 0xfffff

/* Return true if the checksum for the RSDP structure validates, else false. */
static int verify_checksum(uint8_t *rsdp) {
	uint8_t checksum = 0;
	for(int i = 0; i < 20; i++) {
		checksum += rsdp[i];
	}
	return checksum == 0;
}

acpi_RSDP *acpi_find_rsdp(void) {
	uintptr_t cursor = RSDP_START_SEARCH;
	while(cursor <= RSDP_END_SEARCH) {
		if(strncmp((char *)cursor, "RSD PTR ", 8) == 0
				&& verify_checksum((uint8_t *)cursor)) {
			return (acpi_RSDP *)cursor;
		}
		cursor += 16;
	}
	return NULL;
}
