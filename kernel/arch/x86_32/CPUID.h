#pragma once

#include <libsystem/Types.h>

namespace hegel::arch::x86 {

extern "C" uint32_t cpuid_supported(void);

extern "C" void cpuid_get_features(uint32_t* ecx, uint32_t* edx);
extern "C" void cpuid_vendor_code(char* buf); // a 13 char buffer to copy code into

const char* cpuid_vendor_name(void);
void cpuid_check_system_requirements(void);

}
