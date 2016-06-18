OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(main)

SECTIONS
{
  . = 0x23F00000;
  start_addr = .;
  .text.start : { *(.text.start) }
  .text       : { *(.text) *(.text*) }
  .rodata     : { *(.rodata) *(.rodata*) }
  .data       : { *(.data) *(.data*) }
  .bss        : { *(.bss) *(.bss*) }
  total_size = . - start_addr;
}