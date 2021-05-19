#pragma once

#include <libsystem/Types.h>
#include <libsystem/Format.h>

namespace hegel::memory {

class MemoryRegion : public Format
{
private:
    uintptr_t _base_page;
    size_t _page_count;

    MemoryRegion(uintptr_t base_page, size_t page_count);

public:
    MemoryRegion();
    virtual ~MemoryRegion() {}

    static MemoryRegion empty();
    static MemoryRegion from_non_aligned_address(uintptr_t addr, size_t size);
    static MemoryRegion from_aligned_address(uintptr_t addr, size_t size);
    static MemoryRegion create_around_non_aligned_address(uintptr_t addr, size_t size);
    static MemoryRegion from_page(uintptr_t page, size_t count);

    void merge(MemoryRegion& other);
    MemoryRegion take(size_t page_count);
    MemoryRegion half_under(MemoryRegion split);
    MemoryRegion half_over(MemoryRegion split);

    bool is_overlapping_with(MemoryRegion other);
    bool is_continuous_with(MemoryRegion other);
    bool is_empty();

    uintptr_t base_address();
    uintptr_t end_address();

    size_t size();

    uintptr_t base_page();
    uintptr_t end_page();
    size_t page_count();

    bool operator==(MemoryRegion& other);

    /*ErrorOrSizeT format(Stream& stream, FormatInfo& info) 
    {
        __unused(info);

        return hegel::format(stream, "MemoryRegion({#x} - {#x})", base_address(), end_address() - 1);
    }*/
};

}