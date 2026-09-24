ENTRY(_start)

SECTIONS {
  . = 0x80200000;

  .text   : { *(.text.boot) *(.text .text.*) }
  .rodata : ALIGN(4) { *(.rodata .rodata.*) }
  .data   : ALIGN(4) { *(.data .data.*) }

  .bss : ALIGN(4) {
    __bss_start = .;
    *(.bss .bss.* COMMON)
    __bss_end = .;
  }

  . = ALIGN(16);
  . += 64 * 1024;
  __stack_top = .;

  . = ALIGN(4096);
  PROVIDE(_end = .);
}
