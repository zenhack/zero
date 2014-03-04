#include <kernel/panda/mmio.h>
#include <kernel/panda/uart.h>

void uart_clear(uint32_t uart_base) {
	/* this is the procedure outlined in [omap4460] section 23.3.5.1.1.1 */
	uint32_t sysc = uart_base + UART_SYSC;
	uint32_t syss = uart_base + UART_SYSS;
	put32(sysc, get32(sysc) | UART_SYSC_SOFTRESET);
	while(get32(syss) & UART_SYSS_RESETDONE);
}

void uart_enable_fifos_dma(uint32_t uart_base) {
	/* procedure from [omap4460] section 23.3.5.1.1.2 */
	uint32_t uart_lcr = get32(uart_base + UART_LCR);
	put32(uart_base + UART_LCR, 0x00bf);

	uint32_t uart_efr = get32(uart_base + UART_EFR);
	put32(uart_base + UART_EFR, uart_efr | UART_EFR_ENHANCED_EN);

	put32(uart_base + UART_LCR, uart_lcr);
}
