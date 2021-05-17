
CONFIGS = \
	CONFIG \
	CONFIG_ARCH \
	CONGIG_COMPILER \
	CONFIG_BUILD_DIRECTORY \
	CONFIG_DISPLAY \
	CONFIG_VMACHINE \
	CONFIG_LOADER \
	CONFIG_LOG \
	CONFIG_MEMORY \
	CONFIG_NAME \
	CONFIG_OPTIMIZATIONS \
	CONFIG_VERSION \
	CONFIG_IS_TEST \
	CONFIG_IS_RELEASE

CONFIGS_PASS_TO_COMPILER = \
	CONFIG \
	CONFIG_ARCH \
	CONFIG_TOOLCHAIN \
	CONFIG_NAME \
	CONFIG_VERSION \
	CONFIG_IS_TEST \
	CONFIG_IS_RELEASE

CONFIG?=develop

include make/config/user.mk
include make/config/$(CONFIG).mk

# Set the target architecture.
# Possible values: x86_32/x86_64
CONFIG_ARCH?=x86_32

# Set the toolchain to use
# Possible values: gnu/llvm
CONFIG_TOOLCHAIN?=llvm

# Set the directory where output file will be generated.
CONFIG_BUILD_DIRECTORY?=$(shell pwd)/build

# Set the display backend of the virtual machine (if supported).
# Possible values: sdl/gtk
CONFIG_DISPLAY?=gtk

# Set which virtual machine to use when doing `make run`.
# Possible values: qemu/bochs/vbox
CONFIG_VMACHINE?=qemu

# Set the bootloader.
# Possible values: grub, limine
CONFIG_LOADER?=grub

# Enable/disable the logger.
CONFIG_LOG?=true

# Enable/disable stripping of binaries.
CONFIG_STRIP?=true

# How many megabyte of memory is allocated to the virtual machine.
CONFIG_MEMORY?=256

# Set the name of the distribution.
CONFIG_NAME?=hegel

# The optimisation level used by the compiler.
CONFIG_OPTIMIZATIONS?=-O2

# The version number (usualy year.week).
CONFIG_VERSION?=${shell date +'%y.%W'}

# Should unit tests be run when the system start
CONFIG_IS_TEST?=false

# Is it a release
CONFIG_IS_RELEASE?=false

define BUILD_CONFIG_TEMPLATE =
-D__$(1)__=$(if $(filter $($(1)),true),1,$(if $(filter false, $($(1))),0,\""$($(1))"\"))
endef

BUILD_CONFIGS := $(foreach cfg, $(CONFIGS_PASS_TO_COMPILER), $(call BUILD_CONFIG_TEMPLATE,$(cfg)))

ifeq ($(CONFIG_STRIP),false)
	BUILD_CONFIGS += -g
endif

print-config:
	@echo $(BUILD_CONFIGS)
