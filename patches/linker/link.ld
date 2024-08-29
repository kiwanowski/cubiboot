OUTPUT_FORMAT("elf32-powerpc", "elf32-powerpc", "elf32-powerpc")
OUTPUT_ARCH(powerpc:common)
ENTRY(_start)

PHDRS {
    text PT_LOAD;
    data PT_LOAD;
}

MEMORY {
    MRAM (rwx) : ORIGIN = 0x81600000, LENGTH = 1M
    LOWMEM (rwx) : ORIGIN = 0x80100000, LENGTH = 4M
}

/* used by usbgecko */
EXI = 0xCC006800;
DSP = 0xCC005000;

/* used by OS */
OS_CURRENT_THREAD = 0x800000E4;

SECTIONS {
    . = 0x80100000;

    .data_lowmem (NOLOAD) : {
        . = ALIGN(4);
        *(.data_lowmem*)
    } > LOWMEM

    . = 0x81600000;

    .text : {
        *(.text);
        . = ALIGN(32);

        *(.rodata* .data* .bss* .sbss* .gnu.linkonce.r.*);
        . = ALIGN(32);
    } > MRAM : text = 0

    .reloc : {
        *(.reloc*);
        . = ALIGN(32);
    } > MRAM : text

    _patches_end = .;
}
