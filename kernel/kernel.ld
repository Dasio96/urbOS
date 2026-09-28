ENTRY(_start)

SECTIONS {
  . = 0x80200000;

  .text : ALIGN(4096) {
    __text_start = .;
    *(.text.boot) *(.text .text.*)
    __text_end = .;
  }

  .rodata : ALIGN(4096) {
    __rodata_start = .;
    *(.rodata .rodata.*)
    __rodata_end = .;
  }

  .data : ALIGN(4096) {
    __data_start = .;
    *(.data .data.*) *(.sdata .sdata.*)
    __data_end = .;
  }

  .bss : ALIGN(4096) {
    __bss_start = .;
    *(.bss .bss.* COMMON)
    *(.sbss .sbss.*)
    . = ALIGN(4);
    __bss_end = .;
  }

  . = ALIGN(4096);
  . += 64 * 1024;
  __stack_top = .;

  . = ALIGN(4096);
  PROVIDE(_end = .);
}
