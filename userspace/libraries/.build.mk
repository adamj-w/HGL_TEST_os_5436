
define LIB_TEMPLATE = 

$(1)_ARCHIVE ?= $(BUILD_DIRECTORY_LIBS)/lib$($(1)_NAME).a

$(1)_SOURCES += \
	$$(wildcard userspace/libraries/lib$($(1)_NAME)/*.cpp) \
	$$(wildcard userspace/libraries/lib$($(1)_NAME)/*/*.cpp) \
	$$(wildcard userspace/libraries/lib$($(1)_NAME)/*.asm)

$(1)_OBJECTS = $$(patsubst userspace/libraries/%, $(BUILDROOT)/userspace/libraries/%.o, $$($(1)_SOURCES))

$(1)_HEADERS = \
	$$(wildcard userspace/libraries/lib$($(1)_NAME)/*.h) \
	$$(wildcard userspace/libraries/lib$($(1)_NAME)/*/*.h) 

TARGETS += $$($(1)_ARCHIVE)
OBJECTS += $$($(1)_OBJECTS)

ifeq ($(1), C)
HEADERS += $$(patsubst userspace/libraries/libc/%, $(BUILD_DIRECTORY_INCLUDE)/%, $$($(1)_HEADERS))
else
HEADERS += $$(patsubst userspace/libraries/%, $(BUILD_DIRECTORY_INCLUDE)/%, $$($(1)_HEADERS))
endif

$$($(1)_ARCHIVE): $$($(1)_OBJECTS)
	$$(DIRECTORY_GUARD)
	@echo [LIB$(1)] [AR] $$@
	@$(AR) $(ARFLAGS) $$@ $$^

$(BUILDROOT)/userspace/libraries/lib$($(1)_NAME)/%.cpp.o: userspace/libraries/lib$($(1)_NAME)/%.cpp
	$$(DIRECTORY_GUARD)
	@echo [LIB$(1)] [CXX] $$<
	@$(CXX) $(CXXFLAGS) $($(1)_CXXFLAGS) -c -o $$@ $$<

$(BUILDROOT)/userspace/libraries/lib$($(1)_NAME)/%.asm.o: userspace/libraries/lib$($(1)_NAME)/%.asm
	$$(DIRECTORY_GUARD)
	@echo [LIB$(1)] [AS] $$<
	@$(AS) $(ASFLAGS) $($(1)_ASFLAGS) -o $$@ $$<

endef

$(BUILD_DIRECTORY_INCLUDE)/%.h: userspace/libraries/libc/%.h
	$(DIRECTORY_GUARD)
	@cp $< $@

$(BUILD_DIRECTORY_INCLUDE)/%.h: userspace/libraries/%.h
	$(DIRECTORY_GUARD)
	@cp $< $@

-include userspace/libraries/*/.build.mk
$(foreach lib, $(LIBS), $(eval $(call LIB_TEMPLATE,$(lib))))
