#pragma once

#include <kernel/bootdata/Bootdata.h>

namespace modules {

void modules_initialize(const boot::Bootdata* bootdata);

void ramdisk_init(const boot::Module& mod);

}
