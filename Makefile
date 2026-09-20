CROSS ?= riscv64-elf-
CC     = $(CROSS)gcc
GDB   ?= $(CROSS)gdb
QEMU  ?= qemu-system-riscv32

#   make run BIOS=/usr/lib/riscv32-linux-gnu/opensbi/generic/fw_dynamic.bin
BIOS  ?= default

CFLAGS  = -march=rv32imac_zicsr -mabi=ilp32 -std=c11 -O2 -g \
          -Wall -Wextra -ffreestanding -fno-stack-protector -nostdlib \
          -MMD -MP -Iinclude
LDFLAGS = -T kernel/kernel.ld -nostdlib -Wl,-Map=build/kernel.map

SRCS_C := $(wildcard kernel/*.c lib/*.c)
SRCS_S := $(wildcard kernel/*.S)
OBJS   := $(patsubst %.c,build/%.o,$(SRCS_C)) $(patsubst %.S,build/%.o,$(SRCS_S))
KERNEL := build/kernel.elf

QEMUFLAGS = -machine virt -bios $(BIOS) -nographic -serial mon:stdio \
            --no-reboot -kernel $(KERNEL)

all: $(KERNEL)

$(KERNEL): $(OBJS) kernel/kernel.ld
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(KERNEL)
	$(QEMU) $(QEMUFLAGS)

run-log: $(KERNEL)
	$(QEMU) $(QEMUFLAGS) -d int,cpu_reset -D build/qemu.log

debug: $(KERNEL)
	$(QEMU) $(QEMUFLAGS) -s -S

gdb:
	$(GDB) $(KERNEL) -x scripts/gdbinit

clean:
	rm -rf build

-include $(OBJS:.o=.d)

.PHONY: all run run-log debug gdb clean
