#include "ras.h"

static bool equal(const char *left, const char *right)
{
    while (*left && *left == *right) {
        ++left;
        ++right;
    }
    return *left == *right;
}

static void prompt(void)
{
    console_color(0x0a);
    console_write("ras-os> ");
    console_color(0x07);
}

static void execute(char *line)
{
    while (*line == ' ') {
        ++line;
    }
    char *argument = line;
    while (*argument && *argument != ' ') {
        ++argument;
    }
    if (*argument) {
        *argument++ = '\0';
        while (*argument == ' ') {
            ++argument;
        }
    }
    if (!*line) {
        return;
    }
    if (equal(line, "help")) {
        console_write("Commands:\n"
                      "  help     Show this command list\n"
                      "  clear    Clear the screen\n"
                      "  version  Show the kernel version\n"
                      "  about    Describe ras-os\n"
                      "  echo     Print text (example: echo hello)\n"
                      "  halt     Stop the CPU until the VM is reset\n");
    } else if (equal(line, "clear")) {
        console_clear();
    } else if (equal(line, "version")) {
        console_write("ras-os " RAS_VERSION " (i386, BIOS)\n");
    } else if (equal(line, "about")) {
        console_write("ras-os: a kernel built from scratch in C and assembly.\n"
                      "MIT licensed. No filesystem or user processes yet.\n");
    } else if (equal(line, "echo")) {
        console_write(argument);
        console_putc('\n');
    } else if (equal(line, "halt")) {
        console_write("System halted. You can close or reset the virtual machine.\n");
        for (;;) {
            __asm__ volatile ("cli; hlt");
        }
    } else {
        console_write("Unknown command: ");
        console_write(line);
        console_write(". Type 'help'.\n");
    }
}

_Noreturn void shell_run(void)
{
    char line[128];
    size_t length = 0;
    bool overflow = false;
    bool serial_cr = false;
    unsigned int escape = 0;
    prompt();
    for (;;) {
        int c = serial_getc();
        if (c >= 0) {
            /* Ignore ANSI arrow/function-key sequences from serial terminals. */
            if (escape == 1) {
                escape = (c == '[' || c == 'O') ? 2 : 0;
                continue;
            }
            if (escape == 2) {
                if (c >= 0x40 && c <= 0x7e) {
                    escape = 0;
                }
                continue;
            }
            if (c == 27) {
                escape = 1;
                continue;
            }
            if (c == '\n' && serial_cr) {
                serial_cr = false;
                continue;
            }
            serial_cr = (c == '\r');
        } else {
            c = keyboard_getc();
        }
        if (c < 0) {
            __asm__ volatile ("pause");
            continue;
        }
        if (c == '\r' || c == '\n') {
            console_putc('\n');
            line[length] = '\0';
            if (overflow) {
                console_write("Input too long (maximum 127 characters); command discarded.\n");
            } else {
                execute(line);
            }
            length = 0;
            overflow = false;
            prompt();
        } else if ((c == '\b' || c == 127) && !overflow) {
            if (length) {
                --length;
                console_putc('\b');
            }
        } else if (c >= 32 && c <= 126 && !overflow) {
            if (length + 1 < sizeof(line)) {
                line[length++] = (char)c;
                console_putc((char)c);
            } else {
                overflow = true;
            }
        }
    }
}
