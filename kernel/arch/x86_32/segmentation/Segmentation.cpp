#include "Segmentation.h"
#include "../x86.h"

#include <libsystem/Logger.h>

#include <kernel/system/System.h>

namespace hegel::arch::x86 {

TSS gdt_tss;

GDTEntry gdt_entries[GDT_ENTRY_COUNT] = {};

GDTDesc gdt_descriptor = {};

static void segmentation_panic_dump(void*)
{
    GDTDesc desc = {};
    store_gdt(&desc);
    size_t entry_count = desc.size / sizeof(GDTEntry);
    GDTEntry* entries = (GDTEntry*)desc.offset;

    printf("- x86 Segmentation Info\n");

    // TODO: better printing for flags
    for(size_t i = 0; i < entry_count; ++i) {
        printf("\t[%02u] - at %#010X - %#010X\n", i, entries[i].base(), entries[i].base() + entries[i].limit());
        printf("\t     - Access: %08b Flags: %08b\n", entries[i].access, entries[i].flags);
    }
}

void segmentation_initialize()
{
    gdt_tss = TSS();

    logger_info("Populating the GDT");

    gdt_entries[0] = GDTEntry::create(0, 0, 0, 0); // Required null segment

    gdt_entries[1] = GDTEntry::create(0, UINT32_MAX, 
        GDT_PRESENT | GDT_EXECUTABLE | GDT_READWRITE, GDT_FLAGS); // Kernel executable

    gdt_entries[2] = GDTEntry::create(0, UINT32_MAX, 
        GDT_PRESENT | GDT_READWRITE, GDT_FLAGS); // Kernel data

    gdt_entries[3] = GDTEntry::create(0, UINT32_MAX, 
        GDT_PRESENT | GDT_EXECUTABLE | GDT_READWRITE | GDT_USER, GDT_FLAGS); // User exec

    gdt_entries[4] = GDTEntry::create(0, UINT32_MAX, 
        GDT_PRESENT | GDT_READWRITE | GDT_USER, GDT_FLAGS); // User data

    gdt_entries[5] = GDTEntry::create((uint32_t)(&gdt_tss), sizeof(TSS), 
        GDT_PRESENT | GDT_EXECUTABLE | GDT_ACCESSED, TSS_FLAGS); // TSS segment

    gdt_descriptor.size = sizeof(GDTEntry) * GDT_ENTRY_COUNT;
    gdt_descriptor.offset = (uint32_t)(&gdt_entries[0]);

    logger_info("Loading the GDT.");
    x86::load_gdt((uint32_t)&gdt_descriptor);

    hegel::register_panic_dump_callback(segmentation_panic_dump, nullptr);

    logger_info("Successfully loaded GDT.");
}

void set_kernel_stack(uint32_t stack) {
    gdt_tss.esp0 = stack;
}

}