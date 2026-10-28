ENTRY(_start)
SECTIONS
{
    . = 0x10000;
    .text : { KEEP(*(.text.entry)) *(.text .text.*) }
    .rodata : { *(.rodata .rodata.*) }
    .data : { *(.data .data.*) }
    __loaded_end = .;
    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss .bss.*) *(COMMON)
        __bss_end = .;
    }
    /DISCARD/ : { *(.comment) *(.eh_frame*) *(.note*) }
    ASSERT(__loaded_end - 0x10000 <= 32768, "Kernel exceeds bootloader's 32 KiB load window")
    ASSERT(__bss_end < 0x80000, "Kernel overlaps reserved stack memory")
}
