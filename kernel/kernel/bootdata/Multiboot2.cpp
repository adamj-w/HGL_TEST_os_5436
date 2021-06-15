#include "Multiboot2.h"
#include "Bootdata.h"

#include <libsystem/Logger.h>
#include <libc/string.h>

using namespace graphics;

namespace boot {

static Bootdata _bootdata = {};

const char* _multiboot2_tag_name[] = {
    "END",
    "CMDLINE",
    "BOOT_LOADER_NAME",
    "MODULE",
    "BASIC_MEMINFO",
    "BOOTDEV",
    "MMAP",
    "VBE",
    "FRAMEBUFFER",
    "ELF_SECTIONS",
    "APM",
    "EFI32",
    "EFI64",
    "SMBIOS",
    "ACPI_OLD",
    "ACPI_NEW",
    "NETWORK",
    "EFI_MMAP",
    "EFI_BS",
    "EFI32_IH",
    "EFI64_IH",
    "LOAD_BASE_ADDR",
};

Multiboot::Multiboot(uintptr_t addr, uint32_t magic)
{
    if(magic == MULTIBOOT2_BOOTLOADER_MAGIC) {
        _size = *reinterpret_cast<size_t*>(addr);
        _addr = reinterpret_cast<uintptr_t>(addr + 8);
        _magic = magic;
    }
}

const char* Multiboot::bootloader() 
{
    auto* tag = find_tag<multiboot_tag_string>(MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME);

    if(tag) {
        return &tag->string[0];
    } else {
        return "unknown";
    }
}


const char* Multiboot::commandline()
{
    auto* tag = find_tag<multiboot_tag_string>(MULTIBOOT_TAG_TYPE_CMDLINE);

    if(tag) {
        return tag->string;
    } else {
        return "";
    }
}

const void* Multiboot::get_ACPI_rsdp()
{
    auto* tag = find_tag<multiboot_tag_old_acpi>(MULTIBOOT_TAG_TYPE_ACPI_OLD);

    if(tag) {
        return &tag->rsdp[0];
    } else {
        return nullptr;
    }
}

const Framebuffer* Multiboot::get_framebuffer()
{
    auto* tag = find_tag<multiboot_tag_framebuffer>(MULTIBOOT_TAG_TYPE_FRAMEBUFFER);

    if(tag) {
        graphics::Framebuffer* fb = new graphics::Framebuffer;
        fb->address = tag->common.framebuffer_addr;
        fb->width = tag->common.framebuffer_width;
        fb->height = tag->common.framebuffer_height;
        fb->bpp = tag->common.framebuffer_bpp;
        fb->pitch = tag->common.framebuffer_pitch;
        return fb;
    } else {
        return nullptr;
    }
}

static const char* _multiboot2_memory_type_name[] = 
{
    "INVALID",
    "MULTIBOOT_MEMORY_AVAILABLE",
    "MULTIBOOT_MEMORY_RESERVED",
    "MULTIBOOT_MEMORY_ACPI_RECLAIMABLE",
    "MULTIBOOT_MEMORY_NVS",
    "MULTIBOOT_MEMORY_BADRAM"
};

void multiboot2_parse_memory_map(Bootdata* bootdata, struct multiboot_tag_mmap* tag)
{
    for(multiboot_memory_map_t* mmap = tag->entries;
        (uint8_t*)mmap < (uint8_t*)tag + tag->size;
        mmap = (multiboot_memory_map_t*)((uintptr_t)mmap + tag->entry_size))
    {
        assert(bootdata->memory_map_size < MULTIBOOT_MEMORY_MAP_SIZE);

        if((mmap->addr > UINT32_MAX) ||
            (mmap->addr + mmap->len > UINT32_MAX))
        {
            continue;
        }

        if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
            bootdata->memory_usable += mmap->len;
        }

        MemoryMapEntry* entry = &bootdata->memory_map[bootdata->memory_map_size];

        entry->range = memory::MemoryRange::from_non_aligned_address(mmap->addr, mmap->len);

        switch(mmap->type) {
        case MULTIBOOT_MEMORY_AVAILABLE:
            entry->type = MEMORY_MAP_ENTRY_AVAILABLE;
            break;
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
            entry->type = MEMORY_MAP_ENTRY_ACPI_RECLAIMABLE;
            break;
        case MULTIBOOT_MEMORY_NVS:
            entry->type = MEMORY_MAP_ENTRY_NVS;
            break;
        case MULTIBOOT_MEMORY_BADRAM:
            entry->type = MEMORY_MAP_ENTRY_BADRAM;
            break;
        default:
            entry->type = MEMORY_MAP_ENTRY_RESERVED;
            break;
        }

        bootdata->memory_map_size++;

        logger_info("  - Entry type: %s, Size: %#x, Offset: 0x%#x", _multiboot2_memory_type_name[mmap->type], mmap->len, mmap->addr);
    }
}

void multiboot2_parse_module(Bootdata* bootdata, struct multiboot_tag_module* m)
{
    assert(bootdata->modules_size < MULTIBOOT_MODULES_SIZE);

    Module* module = &bootdata->modules[bootdata->modules_size];
    module->range = memory::MemoryRange::around_non_aligned_address(m->mod_start, m->mod_end - m->mod_start);
    strncpy(module->command_line, (const char*)m->cmdline, MULTIBOOT_COMMAND_LINE_SIZE);
    bootdata->modules_size++;
}

const Bootdata* Multiboot::get_bootdata()
{
    strncpy(_bootdata.bootloader, "unknown", MULTIBOOT_BOOTLOADER_NAME_SIZE);
    strncpy(_bootdata.command_line, "", MULTIBOOT_COMMAND_LINE_SIZE);
    
    for_each_tag([](multiboot_tag* tag) -> Iteration {
        logger_info("%#010X: %s", tag, _multiboot2_tag_name[tag->type]);

        switch(tag->type) {
        case MULTIBOOT_TAG_TYPE_ACPI_OLD: {
            multiboot_tag_old_acpi* acpi = (multiboot_tag_old_acpi*)tag;
            _bootdata.acpi_rsdp_address = (uintptr_t)&acpi->rsdp[0];
        } break;

        case MULTIBOOT_TAG_TYPE_MMAP: {
            multiboot2_parse_memory_map(&_bootdata, (multiboot_tag_mmap *)tag);
        } break;

        case MULTIBOOT_TAG_TYPE_MODULE: {
            multiboot2_parse_module(&_bootdata, (multiboot_tag_module*)tag);
        } break;
        }

        return Iteration::CONTINUE;
    });

    return &_bootdata;
}

}