.SUFFIXES:
.DEFAULT_GOAL := all

export LC_ALL=C
export ROOTDIR=.#$(shell pwd)

DIRECTORY_GUARD=@mkdir -p $(@D)

include make/defaults.mk

BUILD_SYSTEM?=hegel
BUILD_DISTRO?=$(BUILD_SYSTEM)-$(CONFIG_ARCH)-$(CONFIG_LOADER)
BUILD_TARGET?=$(BUILD_SYSTEM)-$(CONFIG_ARCH)-$(CONFIG)

BUILD_GITREF=$(shell git rev-parse --abbrev-ref HEAD || echo unknown)@$(shell git rev-parse --short HEAD || echo unknown)
BUILD_UNAME=$(shell uname -s -o -m -r)
DISKS_DIRECTORY=$(CONFIG_BUILD_DIRECTORY)/disks

BUILDROOT=$(CONFIG_BUILD_DIRECTORY)/$(BUILD_TARGET)
SYSROOT=$(BUILDROOT)/root
BOOTROOT=$(DISKS_DIRECTORY)/$(BUILD_DISTRO)-$(CONFIG)
BOOTDISK=$(DISKS_DIRECTORY)/$(BUILD_DISTRO)-$(CONFIG).img
BOOTROOT_GZIP=$(BOOTDISK).gz

RAMDISK=$(BUILDROOT)/ramdisk.tar

DISTRO_DIRECTORY=make/distro/

BUILD_DIRECTORY_LIBS=$(SYSROOT)/usr/lib
BUILD_DIRECTORY_INCLUDE=$(SYSROOT)/usr/include
BUILD_DIRECTORY_BINS=$(SYSROOT)/usr/bin

include make/toolchains/$(CONFIG_ARCH)-$(CONFIG_TOOLCHAIN).mk

BUILD_WARNING+= \
	-Wall -Wextra -Werror

CXX_WARNINGS+= \
	#-Wnon-virtual-dtor \
	#-Woverloaded-virtual

BUILD_INCLUDE+= \
	-I. \
	-Ikernel \
	-Iuserspace \
	-Ilibraries \
	-Ilibraries/libc \

BUILD_DEFINES+= \
	-D__BUILD_ARCH__=\""$(CONFIG_ARCH)"\" \
	-D__BUILD_CONFIG__=\""$(CONFIG)"\" \
	-D__BUILD_SYSTEM__=\""$(BUILD_SYSTEM)"\" \
	-D__BUILD_TARGET__=\""$(BUILD_TARGET)"\" \
	-D__BUILD_GITREF__=\""$(BUILD_GITREF)"\" \
	-D__BUILD_UNAME__=\""$(BUILD_UNAME)"\" \
	-D__BUILD_VERSION__=\""$(CONFIG_VERSION)"\"

CFLAGS+= \
	-std=c11 \
	-MD \
	--sysroot=$(SYSROOT) \
	$(BUILD_WARNING) \
	$(BUILD_INCLUDE) \
	$(BUILD_DEFINES) \
	$(BUILD_CONFIGS)

CXXFLAGS+= \
	-std=c++17 \
	-MD \
	--sysroot=$(SYSROOT) \
	$(BUILD_WARNING) \
	$(CXX_WARNINGS) \
	$(BUILD_INCLUDE) \
	$(BUILD_DEFINES) \
	$(BUILD_CONFIGS)

test2:
	@echo "$(BUILD_DEFINES)"

include kernel/arch/.build.mk
include kernel/kernel/.build.mk

$(BOOTDISK): $(RAMDISK) $(KERNEL_BINARY) $(DISTRO_DIRECTORY)/grub.cfg
	$(DIRECTORY_GUARD)
	@echo [GRUB-MKRESCUE] $@

	@mkdir -p $(BOOTROOT)/boot/grub
	@cp $(DISTRO_DIRECTORY)/grub.cfg $(BOOTROOT)/boot/grub/
	@cp $(RAMDISK) $(BOOTROOT)/boot/ramdisk.tar
	@cp $(KERNEL_BINARY) $(BOOTROOT)/boot/kernel.bin

	@grub-mkrescue -o $@ $(BOOTROOT) || \
	grub2-mkrescue -o $@ $(BOOTROOT)

#SYSROOT_CONTENT=$(shell find sysroot/ -type f)

$(RAMDISK): $(CRTS) $(HEADERS)
	$(DIRECTORY_GUARD)

	@echo [TAR] $@

	@mkdir -p \
		$(SYSROOT)/bin \
		$(SYSROOT)/sbin \
		$(SYSROOT)/include \
		$(SYSROOT)/lib \
		$(SYSROOT)/usr/bin \
		$(SYSROOT)/usr/lib \
		$(SYSROOT)/usr/include \
	
	@cd $(SYSROOT); tar -cf $@ *

$(BOOTDISK_GZIP): $(BOOTDISK)
	@gzip -c $(BOOTDISK) > $(BOOTDISK_GZIP)

.PHONY: all
all: $(BOOTDISK)

.PHONY: run
include make/vms/$(CONFIG_VMACHINE).mk

.PHONY: sync
sync:
	rm $(BOOTDISK) $(RAMDISK)
	make $(BOOTDISK)

.PHONY: clean
clean:
	rm -rf $(BUILDROOT)

.PHONY: clean-all
clean-all:
	rm -rf $(CONFIG_BUILD_DIRECTORY)

-include $(OBJECTS:.o=.d)