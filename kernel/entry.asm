BITS 32
SECTION .text.entry
GLOBAL _start
EXTERN kernel_main
EXTERN __bss_start
EXTERN __bss_end

_start:
    cli
    cld
    mov esp, 0x90000
    xor ebp, ebp
    xor eax, eax
    mov edi, __bss_start
    mov ecx, __bss_end
    sub ecx, edi
    rep stosb
    call kernel_main
.halt:
    cli
    hlt
    jmp .halt

SECTION .text
GLOBAL fault_entry
EXTERN kernel_panic
fault_entry:
    cli
    cld
    ; No return: use a fresh, aligned stack even if the old one is damaged.
    mov esp, 0x90000
    call kernel_panic
    jmp $

SECTION .note.GNU-stack noalloc noexec nowrite progbits
