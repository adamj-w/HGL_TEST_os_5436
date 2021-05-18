CRTS_SOURCES = \
	userspace/arch/$(CONFIG_ARCH)/crt0.s \
	userspace/arch/$(CONFIG_ARCH)/crti.s \
	userspace/arch/$(CONFIG_ARCH)/crtn.s

CRTS = $(patsubst userspace/arch/$(CONFIG_ARCH)/%.s, $(BUILDROOT)/userspace/arch/%.o, $(CRTS_SOURCES))

$(BUILDROOT)/userspace/arch/%.o: userspace/arch/$(CONFIG_ARCH)/%.s
	$(DIRECTORY_GUARD)
	@echo [AS] $<
	@$(AS) $(ASFLAGS) -o $@ $<
