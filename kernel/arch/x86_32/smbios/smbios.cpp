#include "SMBIOS.h"
#include "smbios_types.h"

#include <libsystem/Logger.h>

namespace smbios {

void initialize(MemoryRange range)
{
    EntryPoint* entry = nullptr;
    for(uintptr_t address = range.base(); address < range.end(); address += 16) {
        entry = reinterpret_cast<EntryPoint*>(address);

        if(memcmp(entry->header, "_SM_", 4) == 0 && entry->checksum() == 0) {
            break;
        }
    }

    logger_info("Found SMBIOS at %#010X", entry);
    entry->iterate([](Header* table) -> Iteration {
        logger_info(" - %s (Type=%u, StringCount=%u) ", table->name(), table->type, table->string_table_length());
        for(size_t i = 1; i < table->string_table_length(); i++) {
            logger_info("   - %s", table->string(i));
        }
        return Iteration::CONTINUE;
    });
}

}
