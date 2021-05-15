#include "Multiboot2.h"
#include "Bootdata.h"

#include <libsystem/Logger.h>
#include <libc/string.h>

using namespace hegel::graphics;

namespace hegel::boot {

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

const Bootdata* Multiboot::get_bootdata()
{
    strncpy(_bootdata.bootloader, "unknown", MULTIBOOT_BOOTLOADER_NAME_SIZE);
    strncpy(_bootdata.command_line, "", MULTIBOOT_COMMAND_LINE_SIZE);
    
    for_each_tag([](multiboot_tag* tag) -> Iteration {
        logger_info("{#x}: {}", tag, _multiboot2_tag_name[tag->type]);

        return Iteration::STOP;
    });

    return &_bootdata;
}

}