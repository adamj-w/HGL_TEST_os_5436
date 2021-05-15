KERNEL_SOURCES += \
	$(wildcard kernel/kernel/*.cpp) \
	$(wildcard kernel/kernel/*/*.cpp) \
	$(wildcard kernel/kernel/*/*/*.cpp) \
	$(wildcard kernel/kernel/*.s) \
	$(wildcard kernel/kernel/*/*.s) \
	$(wildcard kernel/kernel/*/*/*.s)

KERNEL_LIBS += libc libsystem libterminal
KERNEL_LIB_SOURCES += \
	$(wildcard libraries/libc/*.cpp) \
	$(wildcard libraries/libsystem/*.cpp) \
	$(wildcard libraries/libterminal/*.cpp)

KERNEL_BINARY=$(BUILDROOT)/kernel.bin

KERNEL_OBJECTS= \
	$(patsubst %, $(BUILDROOT)/%.o, $(KERNEL_SOURCES)) \
	$(patsubst libraries/%, $(BUILDROOT)/kernel/%.o, $(KERNEL_LIB_SOURCES))

KERNEL_CXXFLAGS += \
	$(CXXFLAGS) \
	-fno-rtti \
	-fno-exceptions \
	-ffreestanding \
	-nostdlib \
	-D__KERNEL__

OBJECTS += $(KERNEL_OBJECTS)

define KERNEL_LIB_TEMP =

$$(BUILDROOT)/kernel/$(1)/%.cpp.o: libraries/$(1)/%.cpp
	$$(DIRECTORY_GUARD)
	@echo [KERNEL] [CXX] $$<
	@$$(CXX) $$(KERNEL_CXXFLAGS) -c -o $$@ $$<

endef

$(foreach lib, $(KERNEL_LIBS), $(eval $(call KERNEL_LIB_TEMP,$(lib))))

$(BUILDROOT)/kernel/kernel/%.cpp.o: kernel/kernel/%.cpp
	$(DIRECTORY_GUARD)
	@echo [KERNEL] [CXX] $<
	@$(CXX) $(KERNEL_CXXFLAGS) -c -o $@ $<

$(KERNEL_BINARY): $(KERNEL_OBJECTS)
	$(DIRECTORY_GUARD)
	@echo [KERNEL] [LD] $(KERNEL_BINARY)
	$(LD) $(LDFLAGS) $(KERNEL_LDFLAGS) -T kernel/arch/$(CONFIG_ARCH)/link.ld -o $@ $^ -nostdlib 
