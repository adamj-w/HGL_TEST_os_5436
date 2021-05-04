BUILD_ARCH?=x86
BUILD_CONFIG?=debug
BUILD_SYSTEM?=hegel

BUILD_TARGET=$(BUILD_CONFIG)-$(BUILD_ARCH)-$(BUILD_SYSTEM)
BUILD_GITREF=$(shell git rev-parse --abbrev-ref HEAD || echo unknown)/$(shell git rev-parse --short HEAD || echo unknown)
BUILD_UNAME=$(shell uname -s -o -m -r)
BUILD_DIRECTORY=build/$(BUILD_TARGET)

IMAGE_DIRECTORY=$(BUILD_DIRECTORY)/image

SOURCES_DIRECTORY=sources

LOG=echo [$(BUILD_SYSTEM)] 
DIRECTORY_GUARD=@mkdir -p $(@D)

SYSTEM_IMAGE=$(BUILD_DIRECTORY)/image.iso
SYSTEM_ROOT=$(BUILD_DIRECTORY)/root

CINCLUDES=-I. \
		  -I$(SOURCES_DIRECTORY) \
		  -I$(LIBRARIES_DIRECTORY)

CDEFINES=-D__BUILD_TARGET__=\""$(BUILD_TARGET)"\" \
		 -D__BUILD_GITREF__=\""$(BUILD_GITREF)"\" \
		 -D__BUILD_UNAME__=\""$(BUILD_UNAME)"\"

CWARNINGS= -Wall -Wextra -Werror

COMMON_CXX=clang++
COMMON_CC=clang
COMMON_LD=ld
COMMON_AS=nasm
COMMON_AR=ar

COMMON_CXXFLAGS=--target=i686-pc-none-elf -march=i686 -std=c++17 -MD -O3 $(CWARNINGS) $(CINCLUDES) $(CDEFINES) -fsanitize=undefined -fno-rtti -fno-exceptions
COMMON_CFLAGS  =--target=i686-pc-none-elf -march=i686 -std=c11 -MD -O3 $(CWARNINGS) $(CINCLUDES) $(CDEFINES) -fsanitize=undefined
COMMON_LDFLAGS=
COMMON_ASFLAGS=-f elf32
COMMON_ARFLAGS=

KERNEL_CXXFLAGS=-ffreestanding -fno-stack-protector -nostdlib -nostdinc++
KERNEL_LDFLAGS=-m elf_i386 -T $(ARCH_DIRECTORY)/system.ld
KERNEL_ASFLAGS=-f elf32

LIBRARIES=libruntime \
		  libsystem \
		  libc

LIBRARIES_DIRECTORY=$(SOURCES_DIRECTORY)/libraries

LIBRARIES_SOURCES=$(shell find $(LIBRARIES_DIRECTORY) -name "*.cpp") \
				  $(wildcard $(SOURCES_DIRECTORY)/targets/$(BUILD_SYSTEM)/*.cpp) \
				  $(wildcard $(SOURCES_DIRECTORY)/targets/$(BUILD_SYSTEM)/$(BUILD_ARCH)/*.cpp) \
				  $(wildcard $(SOURCES_DIRECTORY)/targets/$(BUILD_SYSTEM)/$(BUILD_ARCH)/*.s)

LIBRARIES_OBJECTS=$(patsubst $(SOURCES_DIRECTORY)/%, $(BUILD_DIRECTORY)/%.o, $(LIBRARIES_SOURCES))
LIBRARIES_ARCHIVES=$(patsubst %, $(BUILD_DIRECTORY)/libraries/%.a, $(LIBRARIES))

ARCH_DIRECTORY=$(SOURCES_DIRECTORY)/arch/$(BUILD_ARCH)
SYSTEM_DIRECTORY=$(SOURCES_DIRECTORY)/system
TARGET_DIRECTORY=$(SOURCES_DIRECTORY)/targets

KERNEL_SOURCES=$(wildcard $(ARCH_DIRECTORY)/*.s) \
			   $(wildcard $(ARCH_DIRECTORY)/**/*.s) \
			   $(wildcard $(ARCH_DIRECTORY)/*.cpp) \
			   $(wildcard $(ARCH_DIRECTORY)/**/*.cpp) \
			   $(wildcard $(SYSTEM_DIRECTORY)/*.cpp) \
			   $(wildcard $(SYSTEM_DIRECTORY)/**/*.cpp) \
			   $(wildcard $(LIBRARIES_DIRECTORY)/libc/*.cpp) \
			   $(wildcard $(LIBRARIES_DIRECTORY)/libruntime/*.cpp) \
			   $(wildcard $(LIBRARIES_DIRECTORY)/libsystem/*.cpp) \

KERNEL_OBJECTS=$(patsubst $(SOURCES_DIRECTORY)/%, $(BUILD_DIRECTORY)/%.k.o, $(KERNEL_SOURCES))

KERNEL_BINARY=$(BUILD_DIRECTORY)/kernel.elf

-include $(ARCH_DIRECTORY)/config.mk \
		 $(TARGET_DIRECTORY)/$(BUILD_SYSTEM)/config.mk \
		 $(TARGET_DIRECTORY)/$(BUILD_SYSTEM)/$(BUILD_ARCH)/config.mk

.PHONY: all run run-headless clean dump

all: $(SYSTEM_IMAGE)

run: $(SYSTEM_IMAGE)
	qemu-system-x86_64 -serial mon:stdio -cdrom $(SYSTEM_IMAGE)

run-headless: $(SYSTEM_IMAGE)
	qemu-system-x86_64 -serial mon:stdio -cdrom $(SYSTEM_IMAGE) -nographic

clean:
	echo $(BUILD_DIRECTORY)

$(SYSTEM_IMAGE): $(KERNEL_BINARY) $(LIBRARIES_ARCHIVES) grub.cfg

$(KERNEL_BINARY): $(KERNEL_OBJECTS)
	$(DIRECTORY_GUARD)
	$(COMMON_LD) $(KERNEL_LDFLAGS) -o $@ $^
