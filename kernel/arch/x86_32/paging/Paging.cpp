#include "Paging.h"
#include "PagingTypes.h"

#include <kernel/system/System.h>
#include <kernel/memory/Memory.h>
#include <arch/Arch.h>

#include <libsystem/Assert.h>

using namespace paging;

namespace hegel::arch {

PageDirectory _kernel_page_directory __aligned(ARCH_PAGE_SIZE) = {};
PageTable _kernel_page_tables[256] __aligned(ARCH_PAGE_SIZE) = {};

void virtual_initialize()
{
    for(size_t i = 0; i < 256; i++) {
        PageDirectoryEntry* entry = &_kernel_page_directory.entries[i];
        entry->User = 0;
        entry->Write = 1;
        entry->Present = 1;
        entry->PageFrameNumber = (size_t)&_kernel_page_tables[i] / ARCH_PAGE_SIZE;
    }
}

void virtual_memory_enable()
{
    paging_enable();
}

void* kernel_address_space()
{
    return &_kernel_page_directory;
}

bool virtual_present(void* address_space, uintptr_t vaddr)
{
    auto pd = reinterpret_cast<PageDirectory*>(address_space);

    int pd_i = PAGE_DIRECTORY_INDEX(vaddr);
    PageDirectoryEntry& pd_e = pd->entries[pd_i];

    if(!pd_e.Present) return false;

    PageTable& pt = *reinterpret_cast<PageTable*>(pd_e.PageFrameNumber * ARCH_PAGE_SIZE);

    int pt_i = PAGE_TABLE_INDEX(vaddr);
    PageTableEntry& pt_e = pt.entries[pt_i];

    return pt_e.Present;
}

uintptr_t virtual_to_physical(void* address_space, uintptr_t vaddr)
{
    auto pd = reinterpret_cast<PageDirectory*>(address_space);

    int pd_index = PAGE_DIRECTORY_INDEX(vaddr);
    PageDirectoryEntry& pd_entry = pd->entries[pd_index];

    if(!pd_entry.Present) return 0;

    PageTable& pt = *reinterpret_cast<PageTable*>(pd_entry.PageFrameNumber * ARCH_PAGE_SIZE);

    int pt_index = PAGE_TABLE_INDEX(vaddr);
    PageTableEntry& pt_entry = pt.entries[pt_index];

    if(!pt_entry.Present) return 0;

    return (pt_entry.PageFrameNumber * ARCH_PAGE_SIZE) + (vaddr & 0xfff);
}

Result virtual_map(void* address_space, memory::MemoryRange physical_range, uintptr_t vaddr, memory::MemoryFlags flags)
{
    auto& pd = *reinterpret_cast<PageDirectory*>(address_space);

    for(size_t i = 0; i < physical_range.size() / ARCH_PAGE_SIZE; i++) {
        size_t offset = i * ARCH_PAGE_SIZE;

        int pdi = PAGE_DIRECTORY_INDEX(vaddr + offset);
        auto& pde = pd.entries[pdi];
        auto* pt = reinterpret_cast<PageTable*>(pde.PageFrameNumber * ARCH_PAGE_SIZE);

        if(!pde.Present) {
            auto error = memory::alloc_identity(address_space, MEMORY_CLEAR, (uintptr_t*)&pt);
            if(error != Result::SUCCEED) return error;

            pde.Present = 1;
            pde.Write = 1;
            pde.User = 1;
            pde.PageFrameNumber = (uint32_t)(pt) >> 12;
        }

        int pti = PAGE_TABLE_INDEX(vaddr + offset);
        auto& pte = pt->entries[pti];

        pte.Present = 1;
        pte.Write = 1;
        pte.User = flags & MEMORY_USER;
        pte.PageFrameNumber = (physical_range.base() + offset) >> 12;
    }

    paging_invalidate_tlb();

    return Result::SUCCEED;
}

memory::MemoryRange virtual_alloc(void* address_space, memory::MemoryRange prange, memory::MemoryFlags flags)
{
    bool is_user_memory = flags & MEMORY_USER;

    uintptr_t vaddr = 0;
    size_t cur_size = 0;

    for(size_t i = (is_user_memory ? 256 : 1) * 1024; i < (is_user_memory ? 1024 : 256) * 1024; i++) {
        uintptr_t cur_addr = i * ARCH_PAGE_SIZE;

        if(!virtual_present(address_space, cur_addr)) {
            if(cur_size == 0) {
                vaddr = cur_addr;
            }

            cur_size += ARCH_PAGE_SIZE;

            if(cur_size == prange.size()) {
                assert(virtual_map(address_space, prange, vaddr, flags) == Result::SUCCEED);

                return (memory::MemoryRange){vaddr, cur_size};
            }
        } else {
            cur_size = 0;
        }
    }

    hegel::PANIC("Out of virtual memory!\n");
}

void virtual_free(void* address_space, memory::MemoryRange vrange) 
{
    auto& pd = *reinterpret_cast<PageDirectory*>(address_space);

    for(size_t i = 0; i < vrange.size() / ARCH_PAGE_SIZE; i++) {
        size_t offset = i * ARCH_PAGE_SIZE;

        int pdi = PAGE_DIRECTORY_INDEX(vrange.base() + offset);
        auto& pde = pd.entries[pdi];

        if(!pde.Present) continue;

        auto* pt = reinterpret_cast<PageTable*>(pde.PageFrameNumber * ARCH_PAGE_SIZE);

        int pti = PAGE_TABLE_INDEX(vrange.base() + offset);
        auto& pte = pt->entries[pti];

        if(pte.Present) {
            pte.as_uint = 0;
        }
    }

    paging_invalidate_tlb();
}

void address_space_switch(void* address_space)
{
    paging_load_directory(virtual_to_physical(kernel_address_space(), (uintptr_t)address_space));
}

}