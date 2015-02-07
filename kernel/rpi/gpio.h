#ifndef KERNEL_RPI_GPIO_H
#define KERNEL_RPI_GPIO_H

/* This module provides support for working with the GPIO pins. */

#include <stdint.h>
#include <kernel/rpi/mmio.h>

/* Function select modes ([bcm2835] pg 92): */
#define GPIO_F_IN 0
#define GPIO_F_OUT 1
#define GPIO_F_ALT0 4
#define GPIO_F_ALT1 5
#define GPIO_F_ALT2 6
#define GPIO_F_ALT3 7
#define GPIO_F_ALT4 3
#define GPIO_F_ALT5 2 


/* set pin to function select mode `func`. */
void gpio_fsel(int pin, uint32_t func);

/* set the given pin */
void gpio_set(int pin);

/* clear the given pin */
void gpio_clr(int pin);

#endif
