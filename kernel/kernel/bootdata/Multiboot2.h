#pragma once

#include <libutil/Iteration.h>
#include <libsystem/Types.h>
#include <libsystem/Macros.h>

#include <kernel/bootdata/Bootdata.h>

#include <thirdparty/multboot2.h>

namespace graphics {
struct Framebuffer;
}

namespace boot {

class Multiboot
{
private:
    uint32_t _magic;
    uintptr_t _addr;
    size_t _size;

public:
    Multiboot(uintptr_t addr, uint32_t magic);
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

    const char* bootloader();
    const char* commandline();

    const void* get_ACPI_rsdp();
    const graphics::Framebuffer* get_framebuffer();
    const Bootdata* get_bootdata();
};

}