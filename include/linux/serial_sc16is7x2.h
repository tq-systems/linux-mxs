#ifndef LINUX_SPI_SC16IS752_H
#define LINUX_SPI_SC16IS752_H

#define SC16IS7X2_NR_GPIOS 8

struct sc16is7x2_platform_data {
	unsigned int	uartclk;
	/* uart line number of the first channel */
	unsigned	uart_base;
	/* flags for every channel */
	unsigned	uart_flags[2];
#define SC16IS7X2_UARTF_RS485	(1 << 0)	/* enable auto RTS control */
#define SC16IS7X2_UARTF_RTSN	(1 << 1)	/* inverse RTS line */
	/* number assigned to the first GPIO */
	unsigned	gpio_base;
	char		*label;
	/* list of GPIO names (array length = SC16IS7X2_NR_GPIOS) */
	const char	*const *names;
};

#endif
