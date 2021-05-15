-include kernel/arch/$(CONFIG_ARCH)/.build.mk

KERNEL_SOURCES += \
	$(wildcard kernel/arch/$(CONFIG_ARCH)/*.cpp) \
	$(wildcard kernel/arch/$(CONFIG_ARCH)/*/*.cpp) \
	$(wildcard kernel/arch/$(CONFIG_ARCH)/*.s) \
	$(wildcard kernel/arch/$(CONFIG_ARCH)/*/*.s) 

$(BUILDROOT)/kernel/arch/%.cpp.o: kernel/arch/%.cpp
	$(DIRECTORY_GUARD)
	@echo [KERNEL] [CXX] $<
	@$(CXX) $(KERNEL_CXXFLAGS) -c -o $@ $<

$(BUILDROOT)/kernel/arch/%.s.o: kernel/arch/%.s
	$(DIRECTORY_GUARD)
	@echo [KERNEL] [AS] $<
	@$(AS) $(ASFLAGS) -o $@ $<
