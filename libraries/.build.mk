LIBC_HEADERS = \
	$(wildcard libraries/libc/*.h) \
	$(wildcard libraries/libc/*/*.h)

HEADERS += $(patsubst libraries/libc/%, $(BUILD_DIRECTORY_INCLUDE)/%, $(LIBC_HEADERS))

define LIB_TEMPLATE =

$(1)_ARCHIVE ?= $(BUILD_DIRECTORY_LIBS)/lib$($(1)_NAME).a

$(1)_SOURCES += \
	$$(wildcard libraries/lib$($(1)_NAME)/*.cpp) \
	$$(wildcard libraries/lib$($(1)_NAME)/*/*.cpp) \

$(1)_OBJECTS = $$(patsubst libraries/%, $(BUILDROOT)/libraries/%.o, $$($(1)_SOURCES))

$(1)_HEADERS = \
	$$(wildcard libraries/lib$($(1)_NAME)/*.h) \
	$$(wildcard libraries/lib$($(1)_NAME)/*/*.h) 

TARGETS += $$($(1)_ARCHIVE)
OBJECTS += $$($(1)_OBJECTS)

ifeq ($(1), C)
HEADERS += $$(patsubst libraries/libc/%, $(BUILD_DIRECTORY_INCLUDE)/%, $$($(1)_HEADERS))
else
HEADERS += $$(patsubst libraries/%, $(BUILD_DIRECTORY_INCLUDE)/%, $$($(1)_HEADERS))
endif

$$($(1)_ARCHIVE): $$($(1)_OBJECTS)
	$$(DIRECTORY_GUARD)
	@echo [LIB$(1)] [AR] $$@
	@$(AR) $(ARFLAGS) $$@ $$^

$(BUILDROOT)/libraries/lib$($(1)_NAME)/%.cpp.o: libraries/lib$($(1)_NAME)/%.cpp
	$$(DIRECTORY_GUARD)
	@echo [LIB$(1)] [CC] $$<
	@$(CXX) $(CXXFLAGS) $($(1)_CXXFLAGS) -c -o $$@ $$<

test_$($(1)_NAME):


endef

$(BUILD_DIRECTORY_INCLUDE)/%.h: libraries/%.h
	$(DIRECTORY_GUARD)
	cp $< $@

$(BUILD_DIRECTORY_INCLUDE)/%.h: libraries/libc/%.h
	$(DIRECTORY_GUARD)
	cp $< $@

-include libraries/*/.build.mk
$(foreach lib, $(LIBS), $(eval $(call LIB_TEMPLATE,$(lib))))
