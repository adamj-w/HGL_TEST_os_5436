define BINARY_TEMPLATE = 

$(1)_BINARY = $(BUILD_DIRECTORY_BINS)/$($(1)_NAME)/$($(1)_NAME)

$(1)_SOURCES = $$(wildcard userspace/$($(1)_NAME)/*.cpp)

$(1)_OBJECTS = $$(patsubst userspace/%, $$(BUILDROOT)/userspace/%.o, $$($(1)_SOURCES))

TARGETS += $$($(1)_BINARY)
OBJECTS += $$($(1)_OBJECTS)

$$($(1)_BINARY): $$($(1)_OBJECTS)
	$$(DIRECTORY_GUARD)
	@echo [$(1)] [LD] $($(1)_NAME)
	$(LD) $(LDFLAGS) -o $$@ $$($(1)_OBJECTS) -L$(BUILD_DIRECTORY_LIBS) $$(patsubst %, -l%, $$($(1)_LIBS)) 

$$(BUILDROOT)/userspace/$$($(1)_NAME)/%.cpp.o: userspace/$$($(1)_NAME)/%.cpp
	$$(DIRECTORY_GUARD)
	@echo [$(1)] [CXX] $<
	@$(CXX) $(CXXFLAGS) -c -o $$@ $$<

endef

-include userspace/*/.build.mk
ECHO = ECHO
ECHO_NAME = echo
ECHO_LIBS = c
$(eval $(call BINARY_TEMPLATE,$(ECHO)))
