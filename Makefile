OSCC ?= ~/.local/bin/x86_64-synnixos-gcc
# SYSUSR ../../sysroot/usr

CC ?= gcc -std=gnu99
CFLAGS ?= -g -static
LDFLAGS ?= -g -static"

local:
	@$(CC) $(CFLAGS) -o epic8 epic8.c
	@./epic8 rom.bin

snx:
	@$(OSCC) $(CFLAGS) -o epic8 epic8.c
	@cp epic8 ~/SynnixOS/sysroot/usr/bin