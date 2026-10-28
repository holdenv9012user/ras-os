#include "ras.h"

struct __attribute__((packed)) idt_gate {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t reserved;
    uint8_t attributes;
    uint16_t offset_high;
};

struct __attribute__((packed)) idt_pointer {
    uint16_t limit;
    uint32_t base;
};

static struct idt_gate idt[256];
extern void fault_entry(void);

static void interrupts_init(void)
{
    uint32_t handler = (uint32_t)(uintptr_t)fault_entry;
    for (size_t i = 0; i < 256; ++i) {
        idt[i].offset_low = (uint16_t)handler;
        idt[i].selector = 0x08;
        idt[i].reserved = 0;
        idt[i].attributes = 0x8e;
        idt[i].offset_high = (uint16_t)(handler >> 16);
    }
    const struct idt_pointer pointer = {
        .limit = sizeof(idt) - 1,
        .base = (uint32_t)(uintptr_t)idt
    };
    __asm__ volatile ("lidt %0" : : "m"(pointer) : "memory");
    outb(0x21, 0xff);
    outb(0xa1, 0xff);
}

_Noreturn void kernel_panic(void)
{
    console_color(0x0c);
    console_write("\nKernel panic: CPU exception or unexpected interrupt.\n");
    console_write("System halted. Restart the virtual machine.\n");
    for (;;) {
        __asm__ volatile ("cli; hlt");
    }
}

void kernel_main(void)
{
    interrupts_init();
    serial_init();
    outb(0x3f2, 0x0c); /* Stop floppy motors after the loader finishes. */
    console_clear();
    console_color(0x0b);
    console_write("Welcome to ras-os\n");
    console_color(0x07);
    console_write("Version " RAS_VERSION " | custom 32-bit x86 kernel\n");
    console_write("Type 'help' for commands.\n\n");
    shell_run();
}
