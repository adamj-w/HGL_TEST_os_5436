QEMU?=qemu-system-i386

QEMU_DISK=-cdrom $(BOOTDISK)

QEMU_FLAGS= \
	-m $(CONFIG_MEMORY)M \
	-serial mon:stdio \
	-machine q35 \
	-rtc base=localtime \
	$(QEMU_DISK)

ifeq ($(CONFIG_DISPLAY),sdl)
	QEMU_FLAGS+=-display sdl
else
	QEMU_FLAGS+=-display gtk
endif

.PHONY: run
run: $(BOOTDISK)
	@echo [QEMU] $^
	@$(QEMU) $(QEMU_FLAGS) -soundhw all || \
	 $(QEMU) $(QEMU_FLAGS) -device ac97

.PHONY: debug
debug: $(BOOTDISK)
	@echo [QEMU] $^
	@$(QEMU) $(QEMU_FLAGS) -soundhw all -s -S || \
	 $(QEMU) $(QEMU_FLAGS) -device ac97 -s -S
