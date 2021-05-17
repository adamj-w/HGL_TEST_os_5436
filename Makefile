.SUFFIXES:
.DEFAULT_GOAL := all

export LC_ALL=C

DIRECTORY_GUARD=@mkdir -p $(@D)

include make/config/default.mk

BUILD_SYSTEM?=hegel
BUILD_DISTRO?=$(BUILD_SYSTEM)-$(CONFIG_ARCH)-$(CONFIG_LOADER)
BUILD_TARGET?=$(BUILD_SYSTEM)-$(CONFIG_ARCH)-$(CONFIG)

BUILD_GITREF=$(shell git rev-parse --abbrev-ref HEAD || echo unknown)@$(shell git rev-parse --short HEAD || echo unknown)
BUILD_UNAME=$(shell uname -s -o -m -r)
DISKS_DIRECTORY=$(CONFIG_BUILD_DIRECTORY)/disks

BUILDROOT=$(CONFIG_BUILD_DIRECTORY)/$(BUILD_TARGET)
SYSROOT=$(BUILDROOT)/sysroot
BOOTROOT=$(DISKS_DIRECTORY)/$(BUILD_DISTRO)-$(CONFIG)
BOOTDISK=$(DISKS_DIRECTORY)/$(BUILD_DISTRO)-$(CONFIG).img
BOOTDISK_GZIP=$(BOOTDISK).gz

RAMDISK=$(BUILDROOT)/ramdisk.tar

BUILD_DIRECTORY_LIBS=$(SYSROOT)/usr/lib
BUILD_DIRECTORY_INCLUDE=$(SYSROOT)/usr/include
BUILD_DIRECTORY_BINS=$(SYSROOT)/usr/bin
BUILD_DIRECTORY_UTILITIES=$(SYSROOT)/bin

BUILD_WARNING := \
	-Wall -Wextra -Werror

CXX_WARNINGS := 

BUILD_INCLUDE := \
	-I. \
	-Iuserspace/libraries

BUILD_DEFINES := \
	-D__BUILD_ARCH__=\""$(CONFIG_ARCH)"\" \
	-D__BUILD_CONFIG__=\""$(CONFIG)"\" \
	-D__BUILD_SYSTEM__=\""$(BUILD_SYSTEM)"\" \
	-D__BUILD_TARGET__=\""$(BUILD_TARGET)"\" \
	-D__BUILD_GITREF__=\""$(BUILD_GITREF)"\" \
	-D__BUILD_UNAME__=\""$(BUILD_UNAME)"\" \
	-D__BUILD_VERSION__=\""$(CONFIG_VERSION)"\"

CFLAGS= \
	-std=c11 \
	-MD \
	--sysroot=$(SYSROOT) \
	$(CONFIG_OPTIMIZATIONS) \
	$(BUILD_WARNING) \
	$(BUILD_INCLUDE) \
	$(BUILD_DEFINES) \
	$(BUILD_CONFIGS)

CXXFLAGS= \
	-std=c++17 \
	-MD \
	--sysroot=$(SYSROOT) \
	$(CONFIG_OPTIMIZATIONS) \
	$(BUILD_WARNING) \
	$(CXX_WARNINGS) \
	$(BUILD_INCLUDE) \
	$(BUILD_DEFINES) \
	$(BUILD_CONFIGS)

include make/toolchain/$(CONFIG_ARCH)-$(CONFIG_TOOLCHAIN).mk

include kernel/.build.mk

include make/distro/.build.mk

$(RAMDISK): $(CRTS) $(TARGETS) $(HEADERS)
	$(DIRECTORY_GUARD)

	@echo [TAR] $@

	@mkdir -p \
		$(SYSROOT)/bin \
		$(SYSROOT)/usr/bin \
		$(SYSROOT)/usr/lib \
		$(SYSROOT)/usr/include \
		$(SYSROOT)/lib \
		$(SYSROOT)/home

	@cd $(SYSROOT); tar -cf $@ *

$(BOOTDISK_GZIP): $(BOOTDISK)
	@gzip -c $(BOOTDISK) > $(BOOTDISK_GZIP)

.PHONY: all
all: $(BOOTDISK)

.PHONY: distro
distro: $(BOOTDISK_GZIP)

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

.PHONY: clean-fs
clean-fs:
	rm -rf $(SYSROOT)

-include $(OBJECTS:.o=.d)

test:
	@echo "$(CXX) $(CXXFLAGS) -c -o test.o test.cpp"
