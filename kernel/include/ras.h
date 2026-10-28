#ifndef RAS_H
#define RAS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define RAS_VERSION "0.1.0"

static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void serial_init(void);
void serial_putc(char c);
int serial_getc(void);
void console_clear(void);
void console_putc(char c);
void console_write(const char *text);
void console_color(uint8_t color);
int keyboard_getc(void);
_Noreturn void shell_run(void);
_Noreturn void kernel_panic(void);

#endif
