CC:=i686-elf-gcc
CXX:=i686-elf-g++
BUILD_DEFINES += \
	-D__GNU__
LD:=i686-elf-ld
LDFLAGS:= \
	--sysroot=$(SYSROOT)
AR:=i686-elf-ar
ARFLAGS:=rcs
AS=nasm
ASFLAGS=-f elf32
STRIP:=i686-elf-strip
