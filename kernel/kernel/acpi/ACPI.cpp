#include "ACPI.h"
#include "ACPITypes.h"

#include <libsystem/Logger.h>
#include <libc/string.h>

#include "../System.h"

namespace hegel::acpi {

static const RSDPDescriptor* _rsdp_addr;

void initialize(const void* rsdp)
{
    if(!((const RSDPDescriptor*) rsdp)->validate()) {
        logger_error("Failed to validate RSDP at address {#x}!", rsdp);
        return;
    }
    
    _rsdp_addr = (const RSDPDescriptor*) rsdp;

    logger_info("RSDP has OEM of {}", &_rsdp_addr->OEMID[0]);

    RSDT* rsdt = reinterpret_cast<RSDT*>(_rsdp_addr->RsdtAddress);

    if(!rsdt->Header.validate()) {
        logger_error("Failed to validate RSDT at addr {#x}!", rsdt);
    }

    for(size_t i = 0; i < rsdt->child_count(); i++) {
        auto* sdt = rsdt->child(i);
        
        // wrap as c strings
        char tmp1[5], tmp2[7], tmp3[9];
        strncpy(tmp1, sdt->Signature, 4);
        strncpy(tmp2, sdt->OEMID, 6);
        strncpy(tmp3, sdt->OEMTableID, 8);
        tmp1[4] = '\0'; tmp2[6] = '\0'; tmp3[8] = '\0';

        logger_info("ACPI: {} - {} - {}", tmp1, tmp2, tmp3);
    }
}

}