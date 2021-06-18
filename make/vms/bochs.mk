BOCHS?=bochs

.PHONY: run
run: $(BOOTDISK)
	@echo [BOCHS] $^
	@$(BOCHS) -qf make/vms/run.bochs

.PHONY: debug
debug: $(BOOTDISK)
	@echo [BOCHS] $^
	@$(BOCHS) -qf make/vms/debug.bochs

