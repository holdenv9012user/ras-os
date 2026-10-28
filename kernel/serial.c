#include "ras.h"

#define COM1 0x3f8

void serial_init(void)
{
    outb(COM1 + 1, 0x00); /* Polling, no UART interrupts. */
    outb(COM1 + 3, 0x80);
    outb(COM1, 0x01);     /* 115200 baud. */
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03); /* 8 data bits, no parity, 1 stop bit. */
    outb(COM1 + 2, 0xc7);
    outb(COM1 + 4, 0x0b);
}

void serial_putc(char c)
{
    /* A missing UART must not prevent VGA output or halt the kernel. */
    for (unsigned int tries = 0; tries < 100000; ++tries) {
        if (inb(COM1 + 5) & 0x20) {
            outb(COM1, (uint8_t)c);
            return;
        }
    }
}

int serial_getc(void)
{
    return (inb(COM1 + 5) & 1) ? inb(COM1) : -1;
}
