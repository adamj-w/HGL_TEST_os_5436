#pragma once

#include <libsystem/Macros.h>
#include <kernel/bootdata/Bootdata.h>

namespace hegel::acpi {

void initialize(const boot::Bootdata* bootdata);// __unchecked_all;

}