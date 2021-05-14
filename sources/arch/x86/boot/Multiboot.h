#pragma once

//#include <libruntime/Callback.h>
#include <libruntime/Iteration.h>
#include <libsystem/Formattable.h>
#include <kernel/memory/MemoryRegion.h>

#include <thirdparty/multboot2.h>

namespace hegel::arch::x86 {

class MemoryMapEntry : public hegel::Formattable
{
private:
    uintptr_t _addr;
    size_t _size;
    int _type;

public:
    MemoryMapEntry(uintptr_t addr, size_t size, int type)
        : _addr(addr), _size(size), _type(type) {}

    bool is_avail() { return _type == MULTIBOOT_MEMORY_AVAILABLE; }
    bool is_reserved() { return _type == MULTIBOOT_MEMORY_RESERVED; }
    bool is_bad() { return _type == MULTIBOOT_MEMORY_BADRAM; }

    uintptr_t address() { return _addr; }
    size_t size() { return _size; }

    memory::MemoryRegion region()
    {
        return memory::MemoryRegion::from_non_aligned_address(_addr, _size);
    }

    ErrorOr<size_t> format(Stream& stream, FormatInfo& info)
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

        return hegel::format(stream, "MemoryMapEntry({}, {})", region(), multiboot_memory_type_name[_type]);
    } 
};

struct Framebuffer
{
    uintptr_t address;
    size_t width;
    size_t height;
    size_t pitch; // bytes per row
    size_t depth; // bits per pixel
};

class Multiboot
{
private:
    uint32_t _magic;
    uintptr_t _addr;
    size_t _size;

public:
    Multiboot(uintptr_t addr, uint32_t magic)
    {
        if(magic == MULTIBOOT2_BOOTLOADER_MAGIC) {
            _size = *reinterpret_cast<size_t*>(addr);
            _addr = reinterpret_cast<uintptr_t>(addr + 8);
            _magic = magic;
        }
    }

    ~Multiboot() {}

    bool is_valid() { return _magic == MULTIBOOT2_BOOTLOADER_MAGIC; }

    template<typename Callback>
    void for_each_tag(Callback c)
    {
        multiboot_tag* cur = reinterpret_cast<multiboot_tag*>(_addr);

        while(cur->type != MULTIBOOT_TAG_TYPE_END) {
            if(c(cur) == Iteration::STOP) return;

            cur = reinterpret_cast<multiboot_tag*>(__align_up(reinterpret_cast<uintptr_t>(cur) + cur->size, 8));
        }
    }

    template<typename tag>
    tag* find_tag(multiboot_uint32_t type)
    {
        tag* result = NULL;

        for_each_tag([&](multiboot_tag* t) {
            if(t->type == type) {
                result = reinterpret_cast<tag*>(t);

                return Iteration::STOP;
            }

            return Iteration::CONTINUE;
        });

        return result;
    }

    const char* bootloader() 
    {
        auto* tag = find_tag<multiboot_tag_string>(MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME);

        if(tag) {
            return &tag->string[0];
        } else {
            return "unknown";
        }
    }

    template<typename Callback>
    void with_memory_map(Callback callback)
    {
        auto* tag = find_tag<multiboot_tag_mmap>(MULTIBOOT_TAG_TYPE_MMAP);

        if(tag) {
            for(multiboot_memory_map_t* mmap = tag->entries; 
                (uint8_t*)mmap < (uint8_t*)tag + tag->size; 
                mmap = (multiboot_memory_map_t*)((uintptr_t)mmap + tag->entry_size))
            {
                if(callback(MemoryMapEntry((uintptr_t)mmap->addr, mmap->len, mmap->type)) == Iteration::STOP) {
                    return;
                }
            }
        }
    }

    void* get_ACPI_rsdp()
    {
        auto* tag = find_tag<multiboot_tag_old_acpi>(MULTIBOOT_TAG_TYPE_ACPI_OLD);

        if(tag) {
            return &tag->rsdp[0];
        } else {
            return nullptr;
        }
    }

    Framebuffer* get_framebuffer()
    {
        auto* tag = find_tag<multiboot_tag_framebuffer>(MULTIBOOT_TAG_TYPE_FRAMEBUFFER);

        if(tag) {
            Framebuffer* fb = new Framebuffer;
            fb->address = tag->common.framebuffer_addr;
            fb->width = tag->common.framebuffer_width;
            fb->height = tag->common.framebuffer_height;
            fb->depth = tag->common.framebuffer_bpp;
            fb->pitch = tag->common.framebuffer_pitch;
            return fb;
        } else {
            return nullptr;
        }
    }
};

}