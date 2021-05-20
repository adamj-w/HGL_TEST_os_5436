$(BOOTDISK): $(KERNEL_BINARY) $(RAMDISK) $(DISTRO_DIRECTORY)/grub.cfg
	$(DIRECTORY_GUARD)
	@echo [GRUB-MKRESCUE] $@

	@mkdir -p $(BOOTROOT)/boot/grub
	@cp $(DISTRO_DIRECTORY)/grub.cfg $(BOOTROOT)/boot/grub/
	@gzip -c $(RAMDISK) > $(BOOTROOT)/boot/ramdisk.tar.gz
	@$(STRIP) $(KERNEL_BINARY) -o $(BOOTROOT)/boot/kernel.bin
	@gzip -c $(BOOTROOT)/boot/kernel.bin > $(BOOTROOT)/boot/kernel.bin.gz

	@grub-mkrescue -o $@ $(BOOTROOT) || \
	grub2-mkrescue -o $@ $(BOOTROOT)
