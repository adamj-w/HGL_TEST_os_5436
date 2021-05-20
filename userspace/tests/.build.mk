define TEST_TEMPLATE = 

$(1)_BINARY = $(BUILD_DIRECTORY_BINS)/$($(1)_NAME)

$(1)_SOURCES = $$(wildcard userspace/tests/$($(1)_NAME)/*.cpp) \
	$$(wildcard userspace/tests/$($(1)_NAME)/*/*.cpp) \

$(1)_OBJECTS = $$(patsubst userspace/tests/%, $$(BUILDROOT)/userspace/tests/%.o, $$($(1)_SOURCES))

TARGETS += $$($(1)_BINARY)
OBJECTS += $$($(1)_OBJECTS)

$$($(1)_BINARY): $$($(1)_OBJECTS) $(CRTS)
	$$(DIRECTORY_GUARD)
	@echo [$(1)] [LD] $($(1)_NAME)
	@$(LD) $(LDFLAGS) -L$(BUILD_DIRECTORY_LIBS) -o $$@ $$^ $$(patsubst %, -l%, $$($(1)_LIBS))
	
$$(BUILDROOT)/userspace/tests/$$($(1)_NAME)/%.cpp.o: userspace/tests/$$($(1)_NAME)/%.cpp
	$$(DIRECTORY_GUARD)
	@echo [$(1)] [CXX] $$<
	@$(CXX) $(CXXFLAGS) -c -o $$@ $$<

endef

-include userspace/tests/*/.build.mk
$(foreach bin, $(BINS), $(eval $(call TEST_TEMPLATE,$(bin))))
