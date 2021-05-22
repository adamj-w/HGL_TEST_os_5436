#include "ACPI.h"
#include "ACPITypes.h"
#include "../interrupts/LAPIC.h"

#include <libsystem/Logger.h>

#include <kernel/system/System.h>

namespace hegel::acpi {

void madt_initialize(MADT* madt)
{
    logger_info("Found local APIC at %#010X", madt->local_apic);

    lapic_found(madt->local_apic);
}

void initialize(const boot::Bootdata* bootdata)
{
    if(!bootdata->acpi_rsdp_address) {
        logger_warn("No ACPI found on system!");
    }

    const RSDPDescriptor* rsdp = reinterpret_cast<RSDPDescriptor*>(bootdata->acpi_rsdp_address);
    const RSDT* rsdt = reinterpret_cast<RSDT*>(rsdp->RsdtAddress);
    rsdt->print_children();
    const MADT* madt = (MADT*)rsdt->child("APIC");

    madt_initialize(const_cast<MADT*>(madt));
}

}