#pragma once

#include <thirdparty/multboot2.h>

#include <kernel/memory/MemoryRange.h>
#include <kernel/graphics/Graphics.h>

#define MULTIBOOT_BOOTLOADER_NAME_SIZE 64
#define MULTIBOOT_COMMAND_LINE_SIZE 256
#define MULTIBOOT_MEMORY_MAP_SIZE 64
#define MULTIBOOT_MODULES_SIZE 16

namespace boot {

enum MemoryMapEntryType
{
    MEMORY_MAP_ENTRY_AVAILABLE,
    MEMORY_MAP_ENTRY_RESERVED,
    MEMORY_MAP_ENTRY_ACPI_RECLAIMABLE,
    MEMORY_MAP_ENTRY_NVS,
    MEMORY_MAP_ENTRY_BADRAM,
    MEMORY_MAP_ENTRY_KERNEL
};

struct MemoryMapEntry
{
    memory::MemoryRange range{};
    MemoryMapEntryType type;
};

struct Module
{
    memory::MemoryRange range;
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