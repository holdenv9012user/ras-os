#include "ras.h"

/* US keyboard, translated PS/2 scan code set 1 (BIOS/QEMU default). */
static const char unshifted[128] = {
    [0x02] = '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    [0x0e] = '\b',
    [0x10] = 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    [0x1c] = '\n',
    [0x1e] = 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    [0x2b] = '\\',
    [0x2c] = 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    [0x39] = ' '
};

static const char shifted[128] = {
    [0x02] = '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
    [0x1a] = '{', '}',
    [0x27] = ':', '"', '~',
    [0x2b] = '|',
    [0x33] = '<', '>', '?'
};

int keyboard_getc(void)
{
    static bool left_shift;
    static bool right_shift;
    static bool caps_lock;
    static bool caps_down;
    static bool extended;
    static unsigned int pause_bytes;
    uint8_t status = inb(0x64);
    if (!(status & 1)) {
        return -1;
    }
    uint8_t code = inb(0x60);
    if (status & 0xe0) { /* Ignore mouse data and parity/timeout errors. */
        return -1;
    }
    if (pause_bytes) {
        --pause_bytes;
        return -1;
    }
    if (code == 0xe1) {
        pause_bytes = 5;
        return -1;
    }
    if (code == 0xe0) {
        extended = true;
        return -1;
    }
    if (extended) {
        extended = false;
        return code == 0x1c ? '\n' : -1;
    }
    bool released = (code & 0x80) != 0;
    code &= 0x7f;
    if (code == 0x2a) {
        left_shift = !released;
        return -1;
    }
    if (code == 0x36) {
        right_shift = !released;
        return -1;
    }
    if (code == 0x3a) {
        if (!released && !caps_down) {
            caps_lock = !caps_lock;
        }
        caps_down = !released;
        return -1;
    }
    if (released) {
        return -1;
    }
    char c = unshifted[code];
    bool shift = left_shift || right_shift;
    if (c >= 'a' && c <= 'z') {
        if (shift != caps_lock) {
            c = (char)(c - 'a' + 'A');
        }
    } else if (shift && shifted[code]) {
        c = shifted[code];
    }
    return c ? c : -1;
}
