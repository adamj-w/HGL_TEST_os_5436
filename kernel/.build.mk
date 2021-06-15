include kernel/arch/$(CONFIG_ARCH)/.build.mk

KERNEL_SOURCES += \
	$(wildcard kernel/kernel/*.cpp) \
	$(wildcard kernel/kernel/*/*.cpp) \
	$(wildcard kernel/arch/$(CONFIG_ARCH)/*.cpp) \
	$(wildcard kernel/arch/$(CONFIG_ARCH)/*/*.cpp) \

KERNEL_ASSEMBLY_SOURCES += \
	$(wildcard kernel/arch/$(CONFIG_ARCH)/*.s) \
	$(wildcard kernel/arch/$(CONFIG_ARCH)/*/*.s) \

KERNEL_LIBRARIES_SOURCES = \
	$(wildcard userspace/libraries/libterminal/*.cpp) \
	$(wildcard userspace/libraries/libsystem/*.cpp) \
	$(wildcard userspace/libraries/libc/*.cpp) \
	$(wildcard userspace/libraries/libc/printf/*.cpp) \

KERNEL_BINARY = $(BUILDROOT)/kernel.bin

KERNEL_OBJECTS = \
	$(patsubst %.cpp, $(BUILDROOT)/%.cpp.o, $(KERNEL_SOURCES)) \
	$(patsubst %.s, $(BUILDROOT)/%.s.o, $(KERNEL_ASSEMBLY_SOURCES)) \
	$(patsubst userspace/libraries/%.cpp, $(BUILDROOT)/kernel/%.cpp.o, $(KERNEL_LIBRARIES_SOURCES))

KERNEL_CXXFLAGS += \
	$(CXXFLAGS) \
	-Ikernel/ \
	-fno-rtti \
	-ffreestanding \
	-nostdlib \
	-D__KERNEL__ 

OBJECTS += $(KERNEL_OBJECTS)

$(BUILDROOT)/kernel/%.cpp.o: kernel/%.cpp
	$(DIRECTORY_GUARD)
	@echo [KERNEL] [CXX] $<
	@$(CXX) $(KERNEL_CXXFLAGS) -c -o $@ $<

$(BUILDROOT)/kernel/%.cpp.o: userspace/libraries/%.cpp
	$(DIRECTORY_GUARD)
	@echo [KERNEL] [CXX] $<
	@$(CXX) $(KERNEL_CXXFLAGS) -c -o $@ $<

$(BUILDROOT)/kernel/%.s.o: kernel/%.s
	$(DIRECTORY_GUARD)
	@echo [KERNEL] [AS] $<
	@$(AS) $(ASFLAGS) -o $@ $<

$(KERNEL_BINARY): $(KERNEL_OBJECTS)
	$(DIRECTORY_GUARD)
	@echo [KERNEL] [LD] $(KERNEL_BINARY)
	@$(LD) $(LDFLAGS) $(KERNEL_LDFLAGS) -T kernel/arch/$(CONFIG_ARCH)/link.ld -o $@ $^ 
