QEMU?=qemu-system-i386

QEMU_DISK?=-cdrom $(BOOTDISK)

QEMU_FLAGS= \
	-m $(CONFIG_MEMORY)M \
	-serial mon:stdio \
	-rtc base=localtime \
	$(QEMU_DISK)

.PHONY: run-qemu
run: $(BOOTDISK)
	@echo [QEMU] $^
	@$(QEMU) $(QEMU_FLAGS)