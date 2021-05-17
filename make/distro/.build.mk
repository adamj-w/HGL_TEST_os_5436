DISTRO_DIRECTORY:=make/distro/$(BUILD_DISTRO)

-include $(DISTRO_DIRECTORY)/.build.mk

print-distro:
	@echo $(BUILD_DISTRO)
