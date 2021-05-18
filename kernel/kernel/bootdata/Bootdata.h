#pragma once

#include <thirdparty/multboot2.h>

#include <libsystem/Formattable.h>
#include <kernel/memory/MemoryRegion.h>
#include <kernel/graphics/Graphics.h>

#define MULTIBOOT_BOOTLOADER_NAME_SIZE 64
#define MULTIBOOT_COMMAND_LINE_SIZE 256
#define MULTIBOOT_MEMORY_MAP_SIZE 64
#define MULTIBOOT_MODULES_SIZE 16

namespace hegel::boot {

enum MemoryMapEntryType
{
    MEMORY_MAP_ENTRY_AVAILABLE,
    MEMORY_MAP_ENTRY_RESERVED,
    MEMORY_MAP_ENTRY_ACPI_RECLAIMABLE,
    MEMORY_MAP_ENTRY_NVS,
    MEMORY_MAP_ENTRY_BADRAM,
    MEMORY_MAP_ENTRY_KERNEL
};

struct MemoryMapEntry : public hegel::Formattable
{
    uintptr_t addr;
    size_t size;
    int type;

    bool is_avail() { return type == MULTIBOOT_MEMORY_AVAILABLE; }
    bool is_reserved() { return type == MULTIBOOT_MEMORY_RESERVED; }
    bool is_bad() { return type == MULTIBOOT_MEMORY_BADRAM; }

    memory::MemoryRegion region()
    {
        return memory::MemoryRegion::from_non_aligned_address(addr, size);
    }

    ErrorOrSizeT format(Stream& stream, FormatInfo& info)
    {
        __unused(info);

        const char* multiboot_memory_type_name[] = {
            "INVALID",
            "AVAILABLE",
            "RESERVED",
            "ACPI_RECLAIMABLE",
            "NVS",
            "BADRAM",
        };

        return hegel::format(stream, "MemoryMapEntry({}, {})", region(), multiboot_memory_type_name[type]);
    } 
};

struct Module
{
    memory::MemoryRegion range;
    char command_line[MULTIBOOT_COMMAND_LINE_SIZE];
};

struct Bootdata
{
    char bootloader[MULTIBOOT_BOOTLOADER_NAME_SIZE];
    char command_line[MULTIBOOT_COMMAND_LINE_SIZE];

    size_t memory_usable;

    MemoryMapEntry memory_map[MULTIBOOT_MEMORY_MAP_SIZE];
    size_t memory_map_size;

    Module modules[MULTIBOOT_MODULES_SIZE];
    size_t modules_size;

    graphics::Framebuffer framebuffer;

    uintptr_t acpi_rsdp_address;
};

}