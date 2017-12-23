#ifndef KERNEL_PANDA_UART_H
#define KERNEL_PANDA_UART_H

/* UART base addresses. From [omap4460] section 23.3.6.1 */
#define UART1_BASE 0x4806a000
#define UART2_BASE 0x4806c000
#define UART3_BASE 0x48020000
#define UART4_BASE 0x4806e000

/* Register offsets. From [omap4460] section 23.3.6.2 */
#define UART_EFR  0x08
#define UART_LCR  0x0c
#define UART_SYSC 0x54
#define UART_SYSS 0x58

/* 23.2.5.1.1 */
#define UART_SYSC_SOFTRESET (1<<1)
#define UART_SYSS_RESETDONE (1<<0)

#define UART_EFR_ENHANCED_EN (1<<4)
/* clears all the uart registers */
void clear_uart(uint32_t uart_base);

#endif
