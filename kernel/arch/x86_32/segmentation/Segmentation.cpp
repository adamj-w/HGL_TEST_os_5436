#include "Segmentation.h"
#include "../x86.h"

#include <libsystem/Logger.h>

namespace hegel::arch::x86 {

TSS gdt_tss;

GDTEntry gdt_entries[GDT_ENTRY_COUNT] = {};

GDTDesc gdt_descriptor = {};

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

    logger_info("Successfully loaded GDT.");
}

}