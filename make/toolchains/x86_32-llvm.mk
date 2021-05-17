CC:=clang
CXX:=clang++
BUILD_DEFINES += \
	-D__LLVM__
TOOLCHAIN_FLAGS = \
	--target=i686-pc-none-elf -march=i686
LD:=i686-elf-ld
LDFLAGS:= \
	--sysroot=$(SYSROOT)
AR:=i686-elf-ar
ARFLAGS:=rcs
AS=nasm
ASFLAGS=-f elf32
STRIP:=i686-elf-strip
