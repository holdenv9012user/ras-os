#include "ras.h"

#define WIDTH 80
#define HEIGHT 25

static volatile uint16_t *const vga = (volatile uint16_t *)0xb8000;
static size_t row;
static size_t column;
static uint8_t color = 0x07;

static uint16_t cell(char c)
{
    return (uint16_t)((uint16_t)color << 8) | (uint8_t)c;
}

static void cursor_update(void)
{
    uint16_t position = (uint16_t)(row * WIDTH + column);
    outb(0x3d4, 14);
    outb(0x3d5, (uint8_t)(position >> 8));
    outb(0x3d4, 15);
    outb(0x3d5, (uint8_t)position);
}

static void scroll(void)
{
    if (row < HEIGHT) {
        return;
    }
    for (size_t i = 0; i < (HEIGHT - 1) * WIDTH; ++i) {
        vga[i] = vga[i + WIDTH];
    }
    row = HEIGHT - 1;
    for (size_t i = row * WIDTH; i < HEIGHT * WIDTH; ++i) {
        vga[i] = cell(' ');
    }
}

void console_color(uint8_t value)
{
    color = value;
}

void console_clear(void)
{
    row = 0;
    column = 0;
    for (size_t i = 0; i < WIDTH * HEIGHT; ++i) {
        vga[i] = cell(' ');
    }
    const char *sequence = "\033[2J\033[H";
    while (*sequence) {
        serial_putc(*sequence++);
    }
    cursor_update();
}

void console_putc(char c)
{
    if (c == '\n') {
        column = 0;
        ++row;
        serial_putc('\r');
        serial_putc('\n');
    } else if (c == '\b') {
        if (column > 0 || row > 0) {
            if (column == 0) {
                --row;
                column = WIDTH;
            }
            --column;
            vga[row * WIDTH + column] = cell(' ');
            serial_putc('\b');
            serial_putc(' ');
            serial_putc('\b');
        }
    } else {
        vga[row * WIDTH + column] = cell(c);
        serial_putc(c);
        if (++column == WIDTH) {
            column = 0;
            ++row;
        }
    }
    scroll();
    cursor_update();
}

void console_write(const char *text)
{
    while (*text) {
        console_putc(*text++);
    }
}
