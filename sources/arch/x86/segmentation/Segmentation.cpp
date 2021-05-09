#include "Segmentation.h"
#include <arch/x86/x86.h>

#include <libsystem/Logger.h>

namespace hegel::arch::x86 {

TSS tss;

GDTEntry entries[GDT_ENTRY_COUNT] = {};

GDTDesc descriptor = {};

void segmentation_initialize()
{
    tss = TSS();

    logger_info("Populating the GDT");

    entries[0] = GDTEntry::create(0, 0, 0, 0); // Required null segment

    entries[1] = GDTEntry::create(0, UINT32_MAX, 
        GDT_PRESENT | GDT_EXECUTABLE | GDT_READWRITE, GDT_FLAGS); // Kernel executable

    entries[2] = GDTEntry::create(0, UINT32_MAX, 
        GDT_PRESENT | GDT_READWRITE, GDT_FLAGS); // Kernel data

    entries[3] = GDTEntry::create(0, UINT32_MAX, 
        GDT_PRESENT | GDT_EXECUTABLE | GDT_READWRITE | GDT_USER, GDT_FLAGS); // User exec

    entries[4] = GDTEntry::create(0, UINT32_MAX, 
        GDT_PRESENT | GDT_READWRITE | GDT_USER, GDT_FLAGS); // User data

    entries[5] = GDTEntry::create((uint32_t)(&tss), sizeof(TSS), 
        GDT_PRESENT | GDT_EXECUTABLE | GDT_ACCESSED, TSS_FLAGS); // TSS segment

    descriptor.size = sizeof(GDTEntry) * GDT_ENTRY_COUNT;
    descriptor.offset = (uint32_t)(&entries[0]);

    logger_info("Loading the GDT.");
    x86::load_gdt((uint32_t)&descriptor);

    logger_info("Successfully loaded GDT.");
}

}