#pragma once

#include <libsystem/Types.h>
#include <libsystem/Macros.h>

namespace paging {

#define PAGE_DIRECTORY_INDEX(vaddr) ((vaddr) >> 22)
#define PAGE_TABLE_INDEX(vaddr) (((vaddr) >> 12) & 0b1111111111) 

#define PAGE_TABLE_ENTRY_COUNT 1024
#define PAGE_DIRECTORY_ENTRY_COUNT 1024

union __packed PageTableEntry 
{
    struct __packed {
        bool Present : 1;
        bool Write : 1;
        bool User : 1;
        bool PageLevelWriteThrough : 1;
        bool PageLevelCacheDisable : 1;
        bool Accessed : 1;
        bool Dirty : 1;
        bool Pat : 1;
        uint32_t Ignored : 4;
        uint32_t PageFrameNumber : 20;
    };

    uint32_t as_uint;
};

struct __packed PageTable
{
    PageTableEntry entries[PAGE_TABLE_ENTRY_COUNT];
};

union __packed PageDirectoryEntry 
{
    struct __packed {
        bool Present : 1;
        bool Write : 1;
        bool User : 1;
        bool PageLevelWriteThrough : 1;
        bool PageLevelCacheDisable : 1;
        bool Accessed : 1;
        bool Ignored1 : 1;
        bool LargePage : 1;
        uint32_t Ignored2 : 4;
        uint32_t PageFrameNumber : 20;
    };

    uint32_t as_uint;
};

struct __packed PageDirectory 
{
    PageDirectoryEntry entries[PAGE_DIRECTORY_ENTRY_COUNT];
};

}